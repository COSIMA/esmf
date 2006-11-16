#if 0
! $Id: ESMF_LocalAllocMacros.h,v 1.9.8.1 2006/11/16 00:15:34 cdeluca Exp $
!
! Earth System Modeling Framework
! Copyright 2002-2008, University Corporation for Atmospheric Research,
! Massachusetts Institute of Technology, Geophysical Fluid Dynamics
! Laboratory, University of Michigan, National Centers for Environmental
! Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
! NASA Goddard Space Flight Center.
! Licensed under the University of Illinois-NCSA License.
!
!==============================================================================
!
#endif
#if 0
!------------------------------------------------------------------------------
! Macros for the Alloc class.
!------------------------------------------------------------------------------
#endif

#if 0
!------------------------------------------------------------------------------
! Allocate the contents of the array.
!------------------------------------------------------------------------------
#endif
#define AllocAllocateMacro(mname, mtypekind, mrank, mdim, mlen, mrng, mloc) \
! <Created by macro - do not edit directly >  @\
        allocate(l##mrank##D##mtypekind%ptr##mrank##D##mtypekind(mrng), stat=status) @\
        if (ESMF_LogMsgFoundAllocError(status, "LocalArray allocate", & @\
                                       ESMF_CONTEXT, rc)) return @\
 @\
        ! Set all the new accumulated information about the array - the @\
        ! F90 pointer, the base addr, the counts, etc. @\
 @\
        ! Set offsets for now to 0, since this is apparently unused. @\
        offsets = 0 @\
 @\
        call c_ESMC_LocalArraySetInternal(array, l##mrank##D##mtypekind, & @\
                        ESMF_DATA_ADDRESS(l##mrank##D##mtypekind%ptr##mrank##D##mtypekind ( mloc )), & @\
                        counts, lbounds, ubounds, offsets, & @\
                        ESMF_TRUE, ESMF_TRUE, status) @\
 @\
        if (ESMF_LogMsgFoundError(status, & @\
                                  ESMF_ERR_PASSTHRU, & @\
                                  ESMF_CONTEXT, rc)) return @\
! < End macro - do not edit directly >  @\

#if 0
!------------------------------------------------------------------------------
! Deallocate the contents of the array.
!------------------------------------------------------------------------------
#endif
#define AllocDeallocateMacro(mname, mtypekind, mrank, mdim, mlen, mrng, mloc) \
! <Created by macro - do not edit directly >  @\
        call c_ESMC_LocalArrayGetF90Ptr(array, l##mrank##D##mtypekind, status) @\
        deallocate(l##mrank##D##mtypekind%ptr##mrank##D##mtypekind, stat=status)  @\
        nullify(l##mrank##D##mtypekind%ptr##mrank##D##mtypekind) @\
! < End macro - do not edit directly >  @\

