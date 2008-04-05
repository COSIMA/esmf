// $Id: ESMC_MeshExodus.h,v 1.1.2.1 2008/04/05 03:13:11 cdeluca Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2008, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.


// (all lines below between the !BOP and !EOP markers will be included in
//  the automated document processing.)
//-------------------------------------------------------------------------
// these lines prevent this file from being read more than once if it
// ends up being included multiple times

#ifndef ESMC_MeshExodus_h
#define ESMC_MeshExodus_h

#include <string>

namespace ESMCI {
namespace MESH {

class Mesh;
void LoadExMesh(Mesh &mesh, const std::string &filename, int nstep = 1);

// Write the mesh to file
void WriteExMesh(const Mesh &mesh, const std::string &filename, int nstep = 1, double tstep = 0.0);

// Write data at a timestep into file (tstep = float)
// If file does not exist, it is created
void WriteExMeshTimeStep(int nstep, double tstep, const Mesh &mesh, const std::string &filename);

} // namespace
} // namespace

#endif
