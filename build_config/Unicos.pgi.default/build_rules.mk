# $Id: build_rules.mk,v 1.4.2.4 2011/03/17 17:43:50 theurich Exp $
#
# Unicos.pgi.default
#

############################################################
# Default compiler setting.
#
ESMF_F90DEFAULT         = ftn
ESMF_CXXDEFAULT         = CC

############################################################
# Default MPI setting.
#
ifeq ($(ESMF_COMM),default)
export ESMF_COMM := mpi
endif

############################################################
# MPI dependent settings.
#
ifeq ($(ESMF_COMM),mpiuni)
# MPI stub library -----------------------------------------
ESMF_F90COMPILECPPFLAGS+= -DESMF_MPIUNI
ESMF_CXXCOMPILECPPFLAGS+= -DESMF_MPIUNI
ESMF_CXXCOMPILEPATHS   += -I$(ESMF_DIR)/src/Infrastructure/stubs/mpiuni
ESMF_MPIRUNDEFAULT      = $(ESMF_DIR)/src/Infrastructure/stubs/mpiuni/mpirun
else
ifeq ($(ESMF_COMM),mpi)
# Vendor MPI -----------------------------------------------
ESMF_F90LINKLIBS       += 
ESMF_CXXLINKLIBS       += 
ESMF_MPIRUNDEFAULT      = mpirun.unicos
ESMF_MPIMPMDRUNDEFAULT  =
else
ifeq ($(ESMF_COMM),user)
# User specified flags -------------------------------------
else
$(error Invalid ESMF_COMM setting: $(ESMF_COMM))
endif
endif
endif

############################################################
# Print compiler version string
#
ESMF_F90COMPILER_VERSION    = ${ESMF_F90COMPILER} -V -v -c
ESMF_CXXCOMPILER_VERSION    = ${ESMF_CXXCOMPILER} -V -v -c

############################################################
# Determine PGI version
#
ESMF_PGIVERSION_MAJOR = $(shell $(ESMF_DIR)/scripts/version.pgi 1 $(ESMF_F90COMPILER_VERSION))
ESMF_F90COMPILECPPFLAGS += -DESMF_PGIVERSION_MAJOR=$(ESMF_PGIVERSION_MAJOR)

ESMF_PGIVERSION_MINOR = $(shell $(ESMF_DIR)/scripts/version.pgi 2 $(ESMF_F90COMPILER_VERSION))
ESMF_F90COMPILECPPFLAGS += -DESMF_PGIVERSION_MINOR=$(ESMF_PGIVERSION_MINOR)

ESMF_PGIVERSION_PATCH = $(shell $(ESMF_DIR)/scripts/version.pgi 3 $(ESMF_F90COMPILER_VERSION))
ESMF_F90COMPILECPPFLAGS += -DESMF_PGIVERSION_PATCH=$(ESMF_PGIVERSION_PATCH)

############################################################
# XT compute nodes do not have support for POSIX IPC (memory mapped files)
#
ESMF_CXXCOMPILECPPFLAGS += -DESMF_NO_POSIXIPC

############################################################
# XT compute nodes do not have support for POSIX dynamic linking
#
ESMF_CXXCOMPILECPPFLAGS += -DESMF_NO_DLFCN

############################################################
# XT compute nodes do not have support for "gethostid()"
#
ESMF_CXXCOMPILECPPFLAGS += -DESMF_NO_GETHOSTID

############################################################
# XT compute nodes do not have support for signals
#
ESMF_CXXCOMPILECPPFLAGS += -DESMF_NO_SIGNALS

############################################################
# XT compute nodes do not have support for system call
#
ESMF_CXXCOMPILECPPFLAGS += -DESMF_NO_SYSTEMCALL

############################################################
# XT compute nodes do not have support for Pthreads
#
ESMF_PTHREADS := OFF

############################################################
# Need this until the file convention is fixed (then remove these two lines)
#
ESMF_F90COMPILEFREENOCPP = -Mfreeform
ESMF_F90COMPILEFIXCPP    = -Mpreprocess -Mnofreeform

############################################################
# Blank out variables to prevent rpath encoding
#
ESMF_F90LINKRPATHS      =
ESMF_CXXLINKRPATHS      =

############################################################
# Determine where pgf90's libraries are located
#
ESMF_CXXLINKPATHS += -L$(shell $(ESMF_DIR)/scripts/libpath.pgf90 $(ESMF_F90COMPILER))

############################################################
# Determine where pgCC's libraries are located
#
ESMF_F90LINKPATHS += -L$(shell $(ESMF_DIR)/scripts/libpath.pgCC $(ESMF_CXXCOMPILER))

############################################################
# Link against libesmf.a using the F90 linker front-end
#
ifeq ($(ESMF_PGIVERSION_MAJOR),7)
ESMF_F90LINKLIBS += -lstd -lrt -lC -ldl
else
ESMF_F90LINKLIBS += -pgcpplibs
endif

############################################################
# Link against libesmf.a using the C++ linker front-end
#
ifeq ($(ESMF_PGIVERSION_MAJOR),7)
ESMF_CXXLINKLIBS += -lrt -ldl
else
ESMF_CXXLINKLIBS += -pgf90libs
endif

############################################################
# Blank out shared library options
#
ESMF_SL_LIBS_TO_MAKE  =
