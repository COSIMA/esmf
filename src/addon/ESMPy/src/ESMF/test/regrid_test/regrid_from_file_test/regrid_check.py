# $Id$

"""
Two Field objects are created, one on a source Mesh and the other
on a destination Mesh.    The source Field is set to an analytic
function, and a regridding operation is performed from the source to
the destination Field.    After the regridding is completed, the
destination Field is compared to the exact solution over that domain.
"""

import sys

import ctypes

from getopt import getopt

try:
    import numpy as np
except:
    raise ImportError('The Numpy library cannot be found!')

try:
    import ESMF
except:
    raise ImportError('The ESMF library cannot be found!')

from regrid_from_file_consts import regrid_method_map, file_type_map, pole_method_map, UNINITVAL, EPSILON
from ESMF.test.regrid_test.grid_regridding_utilities import compare_fields_grid as compare_fields2
from ESMF.test.regrid_test.grid_regridding_utilities import compute_mass_grid as compute_mass

def nc_is_mesh(filename, filetype):
    is_mesh = False
    if (filetype == ESMF.FileFormat.UGRID) or (filetype == ESMF.FileFormat.ESMFMESH):
        is_mesh = True
    elif filetype == ESMF.FileFormat.SCRIP:
        grid_rank = ESMF.ESMP_ScripInqRank(filename)
        if grid_rank == 1:
            is_mesh = True
    return is_mesh
    

def create_grid_or_mesh_from_file(filename, filetype, meshname=None, convert_to_dual=None,
                                  isSphere=None, add_corner_stagger=False, missingvalue=""):
    is_mesh = False
    if nc_is_mesh(filename, filetype):
        print "Creating ESMF.Mesh object"
        grid_or_mesh = ESMF.Mesh(filename=filename,
                         filetype=filetype,
                         meshname=meshname,
                         convert_to_dual=convert_to_dual)
        is_mesh = True
        add_mask = False
    else:
        print "Creating ESMF.Grid object"
        add_mask = len(missingvalue) > 0
        grid_or_mesh = ESMF.Grid(filename=filename, filetype=filetype, 
                                 add_corner_stagger=add_corner_stagger,
                                 is_sphere=isSphere, add_mask=add_mask, 
                                 varname=missingvalue)
    return grid_or_mesh, is_mesh, add_mask

def get_coords_from_grid_or_mesh(grid_or_mesh, is_mesh):
    if is_mesh:
        coords_interleaved, num_nodes, num_dims = ESMF.ESMP_MeshGetCoordPtr(grid_or_mesh)
        lons = np.array([coords_interleaved[2*i] for i in range(num_nodes)])
        lats = np.array([coords_interleaved[2*i+1] for i in range(num_nodes)])
    else:
        lons = grid_or_mesh.get_grid_coords_from_esmc(0, ESMF.StaggerLoc.CENTER,
                                                      ndims=grid_or_mesh.ndims)
        lats = grid_or_mesh.get_grid_coords_from_esmc(1, ESMF.StaggerLoc.CENTER,
                                                      ndims=grid_or_mesh.ndims)
        if grid_or_mesh.ndims == 1:
            lons_1d = lons
            lats_1d = lats
            lons = np.array([[lons_1d[i]]*len(lats_1d) for i in range(len(lons_1d))])
            lats = np.array([lats_1d for lon in lons_1d])
    lons = np.radians(lons)
    lats = np.radians(lats)
    return lons,lats
        
def create_field(grid, name, regrid_method=None, domask=False):
    '''
    PRECONDITIONS: A Mesh or Grid has been created, and 'name' is a string that
                   will be used to initialize the name of a new Field.
    POSTCONDITIONS: A Field has been created.
    '''
    mask_values = None
    if domask:
        mask_values = [0]

    if isinstance(grid,ESMF.Mesh):
        if regrid_method == ESMF.RegridMethod.CONSERVE:
            field = ESMF.Field(grid, name, meshloc=ESMF.MeshLoc.ELEMENT, 
                                mask_values=mask_values)
        else:
            field = ESMF.Field(grid, name, meshloc=ESMF.MeshLoc.NODE, 
                                mask_values=mask_values)
    else:
        field = ESMF.Field(grid, name, mask_values=mask_values)

    return field

def build_analyticfield_const(field):
    '''
    PRECONDITIONS: A Field has been created on the elements of a Mesh or Grid.
    POSTCONDITIONS: The 'field' has been initialized to a constant analytic field.
    '''
    # set the field to a constant value
    for i in range(field.shape[0]):    # this routine assumes this field is on elements
        field[i] = 1.

    return field

def build_analyticfield(field, lons, lats):
    field.data[...] = 2.0 + np.cos(lats[...])**2 * np.cos(2.0*lons[...])
    return field

def run_regridding(srcfield, dstfield, src_mask, dst_mask, 
                   regrid_method, unmapped_action, srcfracfield, 
                   dstfracfield, pole_method=None, regrid_pole_npoints=None):
    '''
    PRECONDITIONS: Two Fields have been created and a regridding operation
                   is desired from 'srcfield' to 'dstfield'.
    POSTCONDITIONS: A regridding operation has set the data on 'dstfield'.
    '''
    # call the regridding functions
    src_mask_vals = None
    dst_mask_vals = None
    if src_mask:
        src_mask_vals = np.array([0])
    if dst_mask:
        dst_mask_vals = np.array([0])

    regridSrc2Dst = ESMF.Regrid(srcfield, dstfield,
                                src_mask_values=src_mask_vals,
                                dst_mask_values=dst_mask_vals,
                                regrid_method=regrid_method,
                                unmapped_action=unmapped_action,
                                src_frac_field=srcfracfield,
                                dst_frac_field=dstfracfield,
                                pole_method=pole_method,
                                regrid_pole_npoints=regrid_pole_npoints)
    dstfield = regridSrc2Dst(srcfield, dstfield, zero_region=ESMF.Region.SELECT)

    return dstfield

def compare_fields(field1, field2, itrp_tol, csrv_tol, parallel=False, 
                   dstfracfield=None, mass1=None, mass2=None, 
                   regrid_method=ESMF.RegridMethod.CONSERVE, uninitval=422397696.):
    '''
    PRECONDITIONS: Two Fields have been created and a comparison of the
                   the values is desired between 'srcfield' and 'dstfield'.
    POSTCONDITIONS: The values on 'srcfield' and 'dstfield' are compared.
    '''
    import numpy.ma as ma

    correct = False
    # verify that the fields are the same size
    assert field1.shape == field2.shape, 'compare_fields: Fields must be the same size!'
    
    # deal with default values for fracfield
    if dstfracfield is None:
        dstfracfield = ma.ones(field1.shape)

    # compute pointwise error measures
    totalErr = 0.0
    max_error = 0.0
    min_error = 1000000.0
    num_nodes = 0

    # allow fields of all dimensions
    field1_flat = np.ravel(field1.data)
    field2_flat = np.ravel(field2.data)
    field2mask_flat = np.ravel(field2.mask)
    dstfracfield_flat = np.ravel(dstfracfield.data)

    for i in range(field2_flat.size):     
        if ((not field2mask_flat[i]) and 
            (field1_flat[i] != uninitval) and 
            (dstfracfield_flat[i] >= 0.999)):
            if (field2_flat.data[i] != 0.0):
                err = abs(field1_flat[i]/dstfracfield_flat[i] - \
                            field2_flat[i])/abs(field2_flat[i])
            else:
                err = abs(field1_flat[i]/dstfracfield_flat[i] - \
                            field2_flat[i])

            if err > 1:
                print field1_flat[i], field2_flat[i], dstfracfield_flat[i]
            num_nodes += 1
            totalErr += err
            if (err > max_error):
                max_error = err
            if (err < min_error):
                min_error = err


    # gather error on processor 0 or set global variables in serial case
    mass1_global = 0
    mass2_global = 0
    if parallel:
        # use mpi4py to collect values
        from mpi4py import MPI
        comm = MPI.COMM_WORLD
        total_error_global = comm.reduce(totalErr, op=MPI.SUM)
        num_nodes_global = comm.reduce(num_nodes, op=MPI.SUM)
        max_error_global = comm.reduce(max_error, op=MPI.MAX)
        min_error_global = comm.reduce(min_error, op=MPI.MIN)
        if (mass1 and mass2):
            mass1_global = comm.reduce(mass1, op=MPI.SUM)
            mass2_global = comm.reduce(mass2, op=MPI.SUM)
    else:
        total_error_global = totalErr
        num_nodes_global = num_nodes
        max_error_global = max_error
        min_error_global = min_error
        if (mass1 and mass2):
            mass1_global = mass1
            mass2_global = mass2

    # compute relative error measures and compare against tolerance values
    itrp = False
    csrv = False
    if ESMF.local_pet() == 0:
        if mass1_global == 0:
            csrv_error_global = abs(mass2_global - mass1_global)
        else:
            csrv_error_global = abs(mass2_global - mass1_global)/abs(mass1_global)

        # compute mean relative error
        if num_nodes != 0:
            total_error_global = total_error_global/num_nodes_global

        # determine if interpolation and conservation are up to spec
        if (total_error_global < itrp_tol):
            itrp = True
        if (csrv_error_global < csrv_tol):
            csrv = True

        # print out diagnostic information
        print "  Mean relative error = "+str(total_error_global)
        print "  Max  relative error = "+str(max_error_global)
        print "  Conservation  error = "+str(csrv_error_global)
        #print "  Min error   = "+str(min_error_global)
        #print "  srcmass     = "+str(mass1_global)
        #print "  dstmass     = "+str(mass2_global)

    # broadcast in parallel case
    if parallel:
        itrp, csrv = MPI.COMM_WORLD.bcast([itrp, csrv],0)

    # print pass or fail
    if (itrp and csrv):
        print "PET{0} - PASS".format(ESMF.local_pet())
        correct = True
    else:
        print "PET{0} - FAIL".format(ESMF.local_pet())

    return correct
def parse_options(options):
        options = options.split()
        opts, args = getopt(options,'it:p:r', ['src_type=', 'dst_type=', 
                                               'src_meshname=', 'dst_meshname=',
                                               'ignore_unmapped', 
                                               'src_regional', 'dst_regional',
                                               'src_missingvalue=','dst_missingvalue='])
        src_type_str = "SCRIP"
        dst_type_str = "SCRIP"
        src_meshname = "Undefined"
        dst_meshname = "Undefined"
        pole_method_str = None
        unmapped_action = ESMF.UnmappedAction.ERROR
        src_regional = False
        dst_regional = False
        src_missingvalue = ""
        dst_missingvalue = ""
        for opt, arg in opts:
            if opt == '--src_type':
                src_type_str = arg
            elif opt == '--dst_type':
                dst_type_str = arg
            elif opt == '--src_meshname':
                src_meshname = arg
            elif opt == '--dst_meshname':
                dst_meshname = arg
            elif opt == '-i' or opt == '--ignore_unmapped':
                unmapped_action = ESMF.UnmappedAction.IGNORE
            elif opt == '-t':
                src_type_str = arg
                dst_type_str = arg
            elif opt == '-p':
                pole_method_str = arg
            elif opt == '-r':
                src_regional = True
                dst_regional = True
            elif opt == '--src_regional':
                src_regional = True
            elif opt == '--dst_regional':
                dst_regional = True
            elif opt == '--src_missingvalue':
                src_missingvalue = arg
            elif opt == '--dst_missingvalue':
                dst_missingvalue = arg
        return (src_type_str, dst_type_str, src_meshname, dst_meshname,
                unmapped_action, pole_method_str, src_regional, dst_regional,
                src_missingvalue, dst_missingvalue)

def regrid_check(src_fname, dst_fname, regrid_method, options, itrp_err, csrv_err):

#    print "\nregrid_weight_gen_check.py: mesh_check()"

    parallel = False
    if ESMF.pet_count() > 1:
        parallel = True

    # Settings for regrid
    (src_type_str, dst_type_str, src_meshname, dst_meshname,
     unmapped_action, pole_method_str, src_regional, dst_regional,
     src_missingvalue, dst_missingvalue) = parse_options(options)
    src_type = file_type_map[src_type_str]
    dst_type = file_type_map[dst_type_str]
    regrid_method = regrid_method_map[regrid_method]
    convert_to_dual = (regrid_method != ESMF.RegridMethod.CONSERVE)
    add_corner_stagger = (regrid_method == ESMF.RegridMethod.CONSERVE)
    src_is_sphere = not src_regional
    dst_is_sphere = not dst_regional
    pole_method = None
    pole_method_npntavg = 1
    if pole_method_str:
        if pole_method_str in pole_method_map:
            pole_method = pole_method_map[pole_method_str]
        else:
            pole_method = ESMF.PoleMethod.NPNTAVG
            pole_method_npntavg = int(pole_method_str)

    src_mask = False
    srcgrid, src_is_mesh, src_mask = create_grid_or_mesh_from_file(src_fname, src_type,
                                                    meshname=src_meshname,
                                                    convert_to_dual=convert_to_dual, 
                                                    isSphere=src_is_sphere,
                                                    add_corner_stagger=add_corner_stagger,
                                                    missingvalue=src_missingvalue)
    dst_mask = False
    dstgrid, dst_is_mesh, dst_mask = create_grid_or_mesh_from_file(dst_fname, dst_type, 
                                                    meshname=dst_meshname,
                                                    convert_to_dual=convert_to_dual, 
                                                    isSphere=dst_is_sphere,
                                                    add_corner_stagger=add_corner_stagger,
                                                    missingvalue=dst_missingvalue)

    # Get node coordinates in radians
    src_lons, src_lats = get_coords_from_grid_or_mesh(srcgrid, src_is_mesh)
    dst_lons, dst_lats = get_coords_from_grid_or_mesh(dstgrid, dst_is_mesh)
    
    # create Field objects on the Grids
    srcfield = create_field(srcgrid, 'srcfield', 
                            regrid_method=regrid_method, domask=src_mask)
    dstfield = create_field(dstgrid, 'dstfield', 
                            regrid_method=regrid_method, domask=dst_mask)
    dstfield2 = create_field(dstgrid, 'dstfield_exact', 
                            regrid_method=regrid_method, domask=dst_mask)

    #create the frac fields
    srcfracfield = create_field(srcgrid, 'src_frac_field', 
                            regrid_method=regrid_method)
    dstfracfield = create_field(dstgrid, 'dst_frac_field', 
                            regrid_method=regrid_method)

    # initialize the Fields to an analytic function
    srcfield = build_analyticfield(srcfield, src_lons, src_lats)
    dstfield2 = build_analyticfield(dstfield2, dst_lons, dst_lats)

    # run the ESMF regridding
    dstfield.data[...] = UNINITVAL
    dstfield = run_regridding(srcfield, dstfield, src_mask, dst_mask,
                              regrid_method, unmapped_action, 
                              srcfracfield, dstfracfield,
                              pole_method=pole_method, 
                              regrid_pole_npoints=pole_method_npntavg)

    srcmass = None
    dstmass = None
    if regrid_method == ESMF.RegridMethod.CONSERVE:
        # create the area fields
        srcareafield = create_field(srcgrid, 'srcareafield', 
                        regrid_method=regrid_method)
        dstareafield = create_field(dstgrid, 'dstareafield', 
                        regrid_method=regrid_method)

        srcmass = compute_mass(srcfield, srcareafield, 
                            dofrac=True, fracfield=srcfracfield)
        dstmass = compute_mass(dstfield, dstareafield, uninitval=UNINITVAL)
    else:
        srcfracfield = None
        dstfracfield = None

    correct = compare_fields(dstfield, dstfield2, itrp_err, csrv_err, 
                              parallel=parallel, dstfracfield=dstfracfield,
                              mass1=srcmass, mass2=dstmass, 
                              regrid_method=regrid_method, uninitval=UNINITVAL)

    return correct

