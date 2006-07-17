# $Id: build_rules.mk,v 1.1.2.1 2006/07/17 18:19:27 theurich Exp $
#
#  Linux.pgi.default
#

############################################################
# Default compiler setting.
#
ESMF_F90DEFAULT         = pgf90
ESMF_CXXDEFAULT         = g++

############################################################
# Default MPI setting.
#
ifeq ($(ESMF_COMM),default)
export ESMF_COMM := mpiuni
endif

############################################################
# MPI dependent settings.
#
ifeq ($(ESMF_COMM),mpiuni)
# MPI stub library -----------------------------------------
ESMF_F90LINKLIBS       += -lmpiuni
ESMF_CXXCOMPILEOPTS    += -DESMF_MPIUNI
ESMF_CXXCOMPILEPATHS   += -I$(ESMF_DIR)/src/Infrastructure/stubs/mpiuni
ESMF_CXXLINKLIBS       += -lmpiuni
ESMF_MPIRUNDEFAULT      = $(ESMF_DIR)/src/Infrastructure/stubs/mpiuni/mpirun
else
ifeq ($(ESMF_COMM),mpich)
# Mpich ----------------------------------------------------
ESMF_F90DEFAULT         = mpif90
ESMF_F90LINKLIBS       += -lpmpich++ -lmpich
ESMF_CXXDEFAULT         = mpiCC
ESMF_CXXCOMPILEOPTS    += -DESMF_MPICH
ESMF_MPIRUNDEFAULT      = mpirun
else
ifeq ($(ESMF_COMM),mpich2)
# Mpich2 ---------------------------------------------------
ESMF_F90DEFAULT         = mpif90
ESMF_CXXDEFAULT         = mpicxx
ESMF_MPIRUNDEFAULT      = mpirun
ESMF_MPIMPMDRUNDEFAULT  = mpiexec
else
ifeq ($(ESMF_COMM),lam)
# LAM (assumed to be built with pgf90) ---------------------
ESMF_F90DEFAULT         = mpif77
ESMF_CXXDEFAULT         = mpic++
ESMF_MPIRUNDEFAULT      = mpirun
ESMF_MPIMPMDRUNDEFAULT  = mpiexec
else
ifeq ($(ESMF_COMM),openmpi)
# OpenMPI --------------------------------------------------
ESMF_F90DEFAULT         = mpif90
ESMF_CXXDEFAULT         = mpicxx
ESMF_MPIRUNDEFAULT      = mpirun
ESMF_MPIMPMDRUNDEFAULT  = mpiexec
else
ifeq ($(ESMF_COMM),user)
# User specified flags -------------------------------------
else
$(error Invalid ESMF_COMM setting: $(ESMF_COMM))
endif
endif
endif
endif
endif
endif

############################################################
#
# location of external libs.  if you want to use any of these,
# define ESMF_SITE to my_site so the build system can find it,
# copy this file into Linux.intel.my_site, and uncomment the
# libs you want included.  remove the rest of this file since
# both this file and the site file will be included.

# LAPACK_INCLUDE   = 
# LAPACK_LIB       = -L/usr/local/lib -llapack
# NETCDF_INCLUDE   = -I/usr/local/include/netcdf
# NETCDF_LIB       = -L/usr/local/lib -lnetcdf
# HDF_INCLUDE      = -I/usr/local/include/hdf
# HDF_LIB          = -L/usr/local/lib/ -lmfhdf -ldf -ljpeg -lz
# BLAS_INCLUDE     = 
# BLAS_LIB         = -latlas -lscs

############################################################
#
# Link against GCC's stdc++ library (because g++ is used)
ESMF_F90LINKPATHS += -L$(dir $(shell gcc -print-file-name=libstdc++.so))
ESMF_F90LINKLIBS  += -lstdc++

############################################################
#
# Compiler options to print version string
ESMF_CXXVOPT        = -V -v
ESMF_F90VOPT        = -V -v

############################################################
#
# Need this until the file convention is fixed (then remove these two lines)
ESMF_F90COMPILEFREENOCPP = -Mfreeform
ESMF_F90COMPILEFIXCPP    = -Mpreprocess -Mnofreeform

############################################################
#
# Determine where pgf90's libraries are located
ESMF_CXXLINKPATHS += -L$(dir $(shell $(ESMF_DIR)/scripts/pgf90-libpath $(ESMF_F90COMPILER)))
ESMF_CXXLINKRPATHS += \
  -Wl,-rpath,$(dir $(shell $(ESMF_DIR)/scripts/pgf90-libpath $(ESMF_F90COMPILER)))

############################################################
#
# Link against libesmf.a using the F90 linker front-end
ESMF_F90LINKLIBS += -lrt -lC -lc

############################################################
#
# Link against libesmf.a using the C++ linker front-end
ESMF_CXXLINKLIBS += -lrt -lC -lpgf90 -lpgf90_rpm1 -lpgf902 -lpgf90rtl -lpgftnrtl

###############################################################################

SL_LIBS_TO_MAKE = 
C_SL_LIBOPTS  = -shared


