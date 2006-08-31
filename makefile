# $Id: makefile,v 1.69.2.11 2006/08/31 20:12:20 theurich Exp $
#===============================================================================
#                            makefile
# 
# This is the primary makefile for building Earth System Modeling Framework
# (ESMF) utilities.  
#===============================================================================

#-------------------------------------------------------------------------------
# The ESMF_DIR environment variable MUST be set by user!!!
#-------------------------------------------------------------------------------
ifndef ESMF_DIR
$(error ESMF_DIR needs to be set to the top ESMF directory)
endif
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# all common rules, definitions are in common.mk
#-------------------------------------------------------------------------------
include $(ESMF_DIR)/build/common.mk
# default target inherited from common.mk is 'lib'
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# things specific to framework build
#-------------------------------------------------------------------------------
DIRS = src

CLEANDIRS = $(ESMF_LIBDIR) $(ESMF_MODDIR) $(ESMF_TESTDIR) $(ESMF_EXDIR) \
	    $(ESMF_BUILD)/src/include
CLOBBERDIRS = $(ESMF_BUILD)/lib $(ESMF_BUILD)/mod \
	      $(ESMF_BUILD)/test $(ESMF_BUILD)/quick_start \
              $(ESMF_BUILD)/release $(ESMF_BUILD)/examples \
              $(ESMF_BUILD)/doc
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Basic targets to build ESMF libraries.
#-------------------------------------------------------------------------------
# Define what building "all" means for the framework.  This needs to be
# after the include of common.mk, so it does not interfere with the
# definition of the default build rule.

all:  lib build_unit_tests build_examples build_system_tests build_demos


#-------------------------------------------------------------------------------
# Prints information about the system and version of ESMF being compiled
#-------------------------------------------------------------------------------
script_info:
	-@echo " "
	-@echo "------------------------------------------"
	-@echo "C++ Compiler version:"; $(ESMF_CXXCOMPILER_VERSION); echo "" 
	-@echo "------------------------------------------"
	-@echo "Fortran Compiler version:"; $(ESMF_F90COMPILER_VERSION); echo ""
	-@echo "------------------------------------------"
	-@if [ -f $(ESMF_DIR)/src/Infrastructure/Util/include/ESMC_Macros.h ] ; then \
	  fgrep ESMF_VERSION_STRING $(ESMF_DIR)/src/Infrastructure/Util/include/ESMC_Macros.h | $(ESMF_SED) "s/^#define //" ; fi
	-@echo "------------------------------------------"
	-@echo " "
	-@echo "Using ESMF environment variables:"
	-@echo "ESMF_DIR: $(ESMF_DIR)"
	-@if [ "$(ESMF_BUILD)" != "$(ESMF_DIR)" ] ; then \
	  echo "ESMF_BUILD: $(ESMF_BUILD)" ; fi
	-@echo "ESMF_OS: $(ESMF_OS)"
	-@echo "ESMF_MACHINE: $(ESMF_MACHINE)"
	-@echo "ESMF_ABI: $(ESMF_ABI)"
	-@echo "ESMF_COMPILER: $(ESMF_COMPILER)"
	-@echo "ESMF_BOPT: $(ESMF_BOPT)"
	-@if [ -n "$(ESMF_OPTLEVEL)" ] ; then \
	  echo "ESMF_OPTLEVEL: $(ESMF_OPTLEVEL)" ; fi
	-@echo "ESMF_COMM: $(ESMF_COMM)"
	-@echo "ESMF_SITE: $(ESMF_SITE)"
	-@echo "ESMF_EXHAUSTIVE: $(ESMF_EXHAUSTIVE)"
	-@if [ -n "$(ESMF_BATCH)" ] ; then \
	  echo "ESMF_BATCH: $(ESMF_BATCH)" ; fi
	-@if [ -n "$(ESMF_BATCHOPTIONS)" ] ; then \
	  echo "ESMF_BATCHOPTIONS: $(ESMF_BATCHOPTIONS)" ; fi
	-@if [ -n "$(ESMF_MPIRUNOPTIONS)" ] ; then \
	  echo "ESMF_MPIRUNOPTIONS: $(ESMF_MPIRUNOPTIONS)" ; fi
	-@if [ -n "$(ESMF_STDCXX_LIBRARY)" ] ; then \
	  echo "ESMF_STDCXX_LIBRARY: $(ESMF_STDCXX_LIBRARY)" ; fi
	-@echo "ESMF_PTHREADS: $(ESMF_PTHREADS)"
	-@if [ -n "$(ESMF_TESTWITHTHREADS)" ] ; then \
	  echo "ESMF_TESTWITHTHREADS: $(ESMF_TESTWITHTHREADS)" ; fi
	-@if [ -n "$(ESMF_NO_IOCODE)" ] ; then \
	  echo "ESMF_NO_IOCODE: $(ESMF_NO_IOCODE)" ; fi
	-@if [ -n "$(ESMF_ARRAY_LITE)" ] ; then \
	  echo "ESMF_ARRAY_LITE: $(ESMF_ARRAY_LITE)" ; fi
	-@if [ -n "$(ESMF_NO_INTEGER_1_BYTE)" ] ; then \
	  echo "ESMF_NO_INTEGER_1_BYTE: $(ESMF_NO_INTEGER_1_BYTE)" ; fi
	-@if [ -n "$(ESMF_NO_INTEGER_2_BYTE)" ] ; then \
	  echo "ESMF_NO_INTEGER_2_BYTE: $(ESMF_NO_INTEGER_2_BYTE)" ; fi
	-@echo " "
	-@echo "------------------------------------------"
	-@echo "ESMF environment variables pointing to 3rd party software:"
	-@if [ -n "$(ESMF_NETCDF_INCLUDE)" ] ; then \
	  echo "ESMF_NETCDF_INCLUDE: $(ESMF_NETCDF_INCLUDE)" ; fi
	-@if [ -n "$(ESMF_NETCDF_LIB)" ] ; then \
	  echo "ESMF_NETCDF_LIB: $(ESMF_NETCDF_LIB)" ; fi
#
info:   script_info
	-@echo " "
	-@echo "------------------------------------------"
	-@echo "Compilers, Linkers, Flags, and Libraries:"
	-@echo "Location of the preprocessor: " `which $(word 1, $(ESMF_CPP))`
	-@echo "Location of the Fortran compiler: " `which $(word 1, $(ESMF_F90COMPILER))`
	-@echo "Location of the Fortran linker: " `which $(word 1, $(ESMF_F90LINKER))`
	-@echo "Location of the C++ compiler: " `which $(word 1, $(ESMF_CXXCOMPILER))`
	-@echo "Location of the C++ linker: " `which $(word 1, $(ESMF_CXXLINKER))`
	-@echo ""
	-@echo "Fortran compiler flags:"
	-@echo "ESMF_F90COMPILEOPTS: $(ESMF_F90COMPILEOPTS)"
	-@echo "ESMF_F90COMPILEPATHS: $(ESMF_F90COMPILEPATHS)"
	-@echo "ESMF_F90COMPILECPPFLAGS: $(ESMF_F90COMPILECPPFLAGS)"
	-@echo ""
	-@echo "Fortran linker flags:"
	-@echo "ESMF_F90LINKOPTS: $(ESMF_F90LINKOPTS)"
	-@echo "ESMF_F90LINKPATHS: $(ESMF_F90LINKPATHS)"
	-@echo "ESMF_F90LINKRPATHS: $(ESMF_F90LINKRPATHS)"
	-@echo "ESMF_F90LINKLIBS: $(ESMF_F90LINKLIBS)"
	-@echo "ESMF_F90ESMFLINKLIBS: $(ESMF_F90ESMFLINKLIBS)"
	-@echo ""
	-@echo "C++ compiler flags:"
	-@echo "ESMF_CXXCOMPILEOPTS: $(ESMF_CXXCOMPILEOPTS)"
	-@echo "ESMF_CXXCOMPILEPATHS: $(ESMF_CXXCOMPILEPATHS)"
	-@echo "ESMF_CXXCOMPILECPPFLAGS: $(ESMF_CXXCOMPILECPPFLAGS)"
	-@echo ""
	-@echo "C++ linker flags:"
	-@echo "ESMF_CXXLINKOPTS: $(ESMF_CXXLINKOPTS)"
	-@echo "ESMF_CXXLINKPATHS: $(ESMF_CXXLINKPATHS)"
	-@echo "ESMF_CXXLINKRPATHS: $(ESMF_CXXLINKRPATHS)"
	-@echo "ESMF_CXXLINKLIBS: $(ESMF_CXXLINKLIBS)"
	-@echo "ESMF_CXXESMFLINKLIBS: $(ESMF_CXXESMFLINKLIBS)"
	-@echo ""
	-@echo ""
	-@echo "------------------------------------------"
	-@echo Compiling on `date` on `hostname`
	-@echo Machine characteristics: `uname -a`
	-@echo "=========================================="
	-@echo " "
#
#
MINFO = $(ESMF_DIR)/build_config/$(ESMF_OS).$(ESMF_COMPILER).$(ESMF_SITE)/machineinfo.h
info_h:
	-@$(ESMF_RM) MINFO $(MINFO)
	-@echo  "static char *machineinfo = \"  " >> MINFO
	-@$(MAKE) -s info >> MINFO 2>&1  
	-@cat MINFO | $(ESMF_SED) -e 's/$$/  \\n\\/' > $(MINFO)
	-@echo  " \"; " >> $(MINFO)
	-@$(ESMF_RM) MINFO

#
#
MKINFO = $(ESMF_LIBDIR)/esmf.mk
info_mk:
	-@$(ESMF_RM) $(MKINFO)
	-@echo "# ESMF application makefile fragment" > $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "# Use the following ESMF_ variables to compile and link" >> $(MKINFO)
	-@echo "# your ESMF application against this ESMF build." >> $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "# !!! VERY IMPORTANT: If the location of this ESMF build is   !!!" >> $(MKINFO)
	-@echo "# !!! changed, e.g. libesmf.a is copied to another directory, !!!" >> $(MKINFO)
	-@echo "# !!! this file - esmf.mk - must be edited to adjust to the   !!!" >> $(MKINFO)
	-@echo "# !!! correct new path                                        !!!" >> $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "# Please see end of file for options used on this ESMF build" >> $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_F90COMPILER=$(ESMF_F90COMPILER)" >> $(MKINFO)
	-@echo "ESMF_F90LINKER=$(ESMF_F90LINKER)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_F90COMPILEOPTS=$(ESMF_F90COMPILEOPTS)" >> $(MKINFO)
	-@echo "ESMF_F90COMPILEPATHS=$(ESMF_F90COMPILEPATHS)" >> $(MKINFO)
	-@echo "ESMF_F90COMPILECPPFLAGS=$(ESMF_F90COMPILECPPFLAGS)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_F90LINKOPTS=$(ESMF_F90LINKOPTS)" >> $(MKINFO)
	-@echo "ESMF_F90LINKPATHS=$(ESMF_F90LINKPATHS)" >> $(MKINFO)
	-@echo "ESMF_F90LINKRPATHS=$(ESMF_F90LINKRPATHS)" >> $(MKINFO)
	-@echo "ESMF_F90LINKLIBS=$(ESMF_F90LINKLIBS)" >> $(MKINFO)
	-@echo "ESMF_F90ESMFLINKLIBS=$(ESMF_F90ESMFLINKLIBS)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_CXXCOMPILER=$(ESMF_CXXCOMPILER)" >> $(MKINFO)
	-@echo "ESMF_CXXLINKER=$(ESMF_CXXLINKER)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_CXXCOMPILEOPTS=$(ESMF_CXXCOMPILEOPTS)" >> $(MKINFO)
	-@echo "ESMF_CXXCOMPILEPATHS=$(ESMF_CXXCOMPILEPATHS)" >> $(MKINFO)
	-@echo "ESMF_CXXCOMPILECPPFLAGS=$(ESMF_CXXCOMPILECPPFLAGS)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "ESMF_CXXLINKOPTS=$(ESMF_CXXLINKOPTS)" >> $(MKINFO)
	-@echo "ESMF_CXXLINKPATHS=$(ESMF_CXXLINKPATHS)" >> $(MKINFO)
	-@echo "ESMF_CXXLINKRPATHS=$(ESMF_CXXLINKRPATHS)" >> $(MKINFO)
	-@echo "ESMF_CXXLINKLIBS=$(ESMF_CXXLINKLIBS)" >> $(MKINFO)
	-@echo "ESMF_CXXESMFLINKLIBS=$(ESMF_CXXESMFLINKLIBS)" >> $(MKINFO)
	-@echo "" >> $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "# !!! The following options were used on this ESMF build !!!" >> $(MKINFO)
	-@echo "#" >> $(MKINFO)
	-@echo "# ESMF_DIR: $(ESMF_DIR)" >> $(MKINFO)
	-@if [ "$(ESMF_BUILD)" != "$(ESMF_DIR)" ] ; then \
	  echo "# ESMF_BUILD: $(ESMF_BUILD)" >> $(MKINFO) ; fi
	-@echo "# ESMF_OS: $(ESMF_OS)" >> $(MKINFO)
	-@echo "# ESMF_MACHINE: $(ESMF_MACHINE)" >> $(MKINFO)
	-@echo "# ESMF_ABI: $(ESMF_ABI)" >> $(MKINFO)
	-@echo "# ESMF_COMPILER: $(ESMF_COMPILER)" >> $(MKINFO)
	-@echo "# ESMF_BOPT: $(ESMF_BOPT)" >> $(MKINFO)
	-@if [ -n "$(ESMF_OPTLEVEL)" ] ; then \
	  echo "# ESMF_OPTLEVEL: $(ESMF_OPTLEVEL)" >> $(MKINFO) ; fi
	-@echo "# ESMF_COMM: $(ESMF_COMM)" >> $(MKINFO)
	-@echo "# ESMF_SITE: $(ESMF_SITE)" >> $(MKINFO)
	-@echo "# ESMF_EXHAUSTIVE: $(ESMF_EXHAUSTIVE)" >> $(MKINFO)
	-@if [ -n "$(ESMF_BATCH)" ] ; then \
	  echo "# ESMF_BATCH: $(ESMF_BATCH)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_BATCHOPTIONS)" ] ; then \
	  echo "# ESMF_BATCHOPTIONS: $(ESMF_BATCHOPTIONS)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_MPIRUNOPTIONS)" ] ; then \
	  echo "# ESMF_MPIRUNOPTIONS: $(ESMF_MPIRUNOPTIONS)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_STDCXX_LIBRARY)" ] ; then \
	  echo "# ESMF_STDCXX_LIBRARY: $(ESMF_STDCXX_LIBRARY)" >> $(MKINFO) ; fi
	-@echo "# ESMF_PTHREADS: $(ESMF_PTHREADS)" >> $(MKINFO)
	-@if [ -n "$(ESMF_TESTWITHTHREADS)" ] ; then \
	  echo "# ESMF_TESTWITHTHREADS: $(ESMF_TESTWITHTHREADS)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_NO_IOCODE)" ] ; then \
	  echo "# ESMF_NO_IOCODE: $(ESMF_NO_IOCODE)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_ARRAY_LITE)" ] ; then \
	  echo "# ESMF_ARRAY_LITE: $(ESMF_ARRAY_LITE)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_NO_INTEGER_1_BYTE)" ] ; then \
	  echo "# ESMF_NO_INTEGER_1_BYTE: $(ESMF_NO_INTEGER_1_BYTE)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_NO_INTEGER_2_BYTE)" ] ; then \
	  echo "# ESMF_NO_INTEGER_2_BYTE: $(ESMF_NO_INTEGER_2_BYTE)" >> $(MKINFO) ; fi
	-@echo "# " >> $(MKINFO)
	-@echo "# ESMF environment variables pointing to 3rd party software:" >> $(MKINFO)
	-@if [ -n "$(ESMF_NETCDF_INCLUDE)" ] ; then \
	  echo "# ESMF_NETCDF_INCLUDE: $(ESMF_NETCDF_INCLUDE)" >> $(MKINFO) ; fi
	-@if [ -n "$(ESMF_NETCDF_LIB)" ] ; then \
	  echo "# ESMF_NETCDF_LIB: $(ESMF_NETCDF_LIB)" >> $(MKINFO) ; fi

# Ranlib on the libraries
ranlib:
	$(ESMF_RANLIB) $(ESMF_LIBDIR)/*.a

# Deletes ESMF libraries
deletelibs: chkopts_basic
	-$(ESMF_RM) $(ESMF_LIBDIR)/*

# ESMF_COUPLED_FLOW/demo target.
ESMF_COUPLED_FLOW: chkopts build_libs chkdir_tests
	cd src/demo/coupled_flow ;\
	$(MAKE) BOPT=$(BOPT) demos

# ESMF_COUPLED_FLOW_uni/demo target.
ESMF_COUPLED_FLOW_uni: chkopts build_libs chkdir_tests
	cd src/demo/coupled_flow ;\
	$(MAKE) BOPT=$(BOPT) demos_uni


# ------------------------------------------------------------------
# All remaining actions are intended for ESMF developers only.
# ESMF users should not generally need to use these commands.


BUILDFILES = build/* build_config/*/*

DOCS	   = build/README build/doc/*

SCRIPTS    = 

# TODO: this target is not so useful right now - because we try
# to build shared libs on many of our platforms.  the executables
# linked against that lib (like the unit tests and examples) then
# store that build directory inside as where to find the shared
# lib at load time.  to move them, we have to know the eventual
# location of the libs (by having something like ESMF_INSTALL_DIR
# defined at the time the executables are linked, so we can add
# an extra -rpath or -L flag to the link to add the install lib
# dir to be searched at load time.
# also, this should just have a single variable and not separate
# ones for the lib, mod, and includes.
# also, there should be options to install just the bare lib,
# the unit tests and quickstart files, and then everything.
install:
	@if [ "$(ESMF_LIB_INSTALL)" != "" ] ; then \
	 cp -fp $(ESMF_LIBDIR)/* $(ESMF_LIB_INSTALL) ; \
	fi
	if [ "$(ESMF_MOD_INSTALL)" != "" ] ; then \
	 cp -fp $(ESMF_MODDIR)/*.mod $(ESMF_MOD_INSTALL) ; \
	fi
	if [ "$(ESMF_H_INSTALL)" != "" ] ; then \
	 cp -fp $(ESMF_BUILD)/src/include/*.h $(ESMF_H_INSTALL) ; \
	fi


# ------------------------------------------------------------------
# add dummy rules here to avoid gnumake trying to remake the actual
# makefiles themselves; this might not be much of an overhead but 
# since we call make so many times recursively and it does the makefile
# remake rule check on each invocation of make, it effectively renders
# gmake -d (debug mode) almost unreadable.  this cuts the remake rule
# output down immensely.  nsc 05nov04

GNUmakefile:
	@echo ;

makefile:
	@echo ;

$(ESMF_DIR)/makefile:
	@echo ;

$(ESMF_DIR)/build/common.mk:
	@echo ;

$(ESMF_DIR)/build_config/$(ESMF_OS).$(ESMF_COMPILER).$(ESMF_SITE)/build_rules.mk:
	@echo ;

