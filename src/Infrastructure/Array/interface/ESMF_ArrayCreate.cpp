! $Id: ESMF_ArrayCreate.cpp,v 1.10.2.3 2007/10/18 02:41:57 cdeluca Exp $
!
! Earth System Modeling Framework
! Copyright 2002-2007, University Corporation for Atmospheric Research, 
! Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
! Laboratory, University of Michigan, National Centers for Environmental 
! Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
! NASA Goddard Space Flight Center.
! Licensed under the University of Illinois-NCSA License.
!
!==============================================================================
^define ESMF_FILENAME "ESMF_ArrayCreate.F90"
!
!     ESMF Array module
      module ESMF_ArrayCreateMod
!
!==============================================================================
!
! This file contains the Array class methods which are automatically
!  generated from macros to handle the type/kind/rank overloading.
!  See ESMF_ArrayBase.F90 for non-macroized entry points.
! class methods.
!
!------------------------------------------------------------------------------
! INCLUDES
! < ignore blank lines below.  they are created by the files which
!   define various macros. >
#include "ESMF_StdCppMacros.h"
#include "ESMF_ArrayCreateMacros.h"
#include "ESMF_AllocMacros.h"
^include "ESMF.h"
!------------------------------------------------------------------------------
! !USES:
      use ESMF_UtilTypesMod
      use ESMF_BaseMod
      use ESMF_LogErrMod
      use ESMF_ArraySpecMod
      use ESMF_LocalArrayMod
      use ESMF_ArrayMod
      implicit none

!------------------------------------------------------------------------------
! !PRIVATE TYPES:
      private

!------------------------------------------------------------------------------
!     ! ESMF_ArrayOrigin
!
!     ! Private flag which indicates the create was initiated on the F90 side.
!     !  This matches an enum on the C++ side and the values must match.
!     !  Update ../include/ESMC_Array.h if you change these values.

      type ESMF_ArrayOrigin
      sequence
      private
        integer :: origin
      end type

      type(ESMF_ArrayOrigin), parameter :: &
                            ESMF_FROM_FORTRAN   = ESMF_ArrayOrigin(1), &
                            ESMF_FROM_CPLUSPLUS = ESMF_ArrayOrigin(2)

!------------------------------------------------------------------------------
!     ! Internal wrapper structures for passing f90 pointers to C++ and
!     ! guaranteeing they are passed by reference on all compilers and all
!     ! platforms.  These are never seen outside this module.
!
      ! < these expand into defined type declarations >
AllTypesMacro(ArrayType)

!------------------------------------------------------------------------------
! !PUBLIC MEMBER FUNCTIONS:

      public ESMF_ArrayCreate, ESMF_ArrayDestroy

      public ESMF_ArrayF90Allocate
      public ESMF_ArrayF90Deallocate
      public ESMF_ArrayConstructF90Ptr    ! needed for C++ callback only

!EOP
      public operator(.eq.), operator(.ne.)

!------------------------------------------------------------------------------
! The following line turns the CVS identifier string into a printable variable.
      character(*), parameter, private :: version = &
      '$Id: ESMF_ArrayCreate.cpp,v 1.10.2.3 2007/10/18 02:41:57 cdeluca Exp $'

!==============================================================================
! 
! INTERFACE BLOCKS
!
!==============================================================================

!BOP
! !IROUTINE: ESMF_ArrayCreate -- Generic interface to create an Array
!
! !INTERFACE:
     interface ESMF_ArrayCreate
!
! !PRIVATE MEMBER FUNCTIONS:
!
        module procedure ESMF_ArrayCreateByList      ! specify TKR
        module procedure ESMF_ArrayCreateBySpec      ! specify ArraySpec
!   
        ! Plus interfaces for each T/K/R 

!EOP
        
       ! < interfaces for each T/K/R >
InterfaceMacro(ArrayCreateByMTPtr)

       ! < interfaces for each T/K/R >
InterfaceMacro(ArrayCreateByFullPtr)


!BOP
! !DESCRIPTION: 
! This interface provides a single (heavily overloaded) entry point for 
!  the various types of {\tt ESMF\_ArrayCreate} functions.   
!
!  There are 3 options for setting the contents of the {\tt ESMF\_Array}
!  at creation time:
!  \begin{description}
!  \item[Allocate Space Only]
!    Data space is allocated but not initialized.  The caller can query
!    for a pointer to the start of the space to address it directly.
!    The caller must not deallocate the space; the
!    {\tt ESMF\_Array} will release the space when it is destroyed.
!  \item[Data Copy]
!    An existing Fortran array is specified and the data contents are copied
!    into new space allocated by the {\tt ESMF\_Array}.
!    The caller must not deallocate the space; the
!    {\tt ESMF\_Array} will release the space when it is destroyed.
!  \item[Data Reference]
!    An existing Fortran array is specified and the data contents reference
!    it directly.  The caller is responsible for deallocating the space;
!    when the {\tt ESMF\_Array} is destroyed it will not release the space.
!  \end{description}
!
!  There are 3 options for 
!  specifying the type/kind/rank of the {\tt ESMF\_Array} data:
!  \begin{description}
!  \item[List]
!    The characteristics of the {\tt ESMF\_Array} are given explicitly
!    by individual arguments to the create function.
!  \item[ArraySpec]
!    A previously created {\tt ESMF\_ArraySpec} object is given which
!    describes the characteristics.
!  \item[Fortran 90 Pointer]
!    An associated or unassociated Fortran 90 array pointer is used to 
!    describe the array.
!    (Only available from the Fortran interface.)
!  \end{description}
!  
!  The concept of an ``empty'' {\tt Array} does not exist.  To make an
!  ESMF object which stores the Type/Kind/Rank information create an
!  {\tt ESMF\_ArraySpec} object which can then be used repeatedly in
!  subsequent {\tt Array} Create calls.
!  
end interface
!EOP 

!==============================================================================

      contains

!==============================================================================
!------------------------------------------------------------------------------

!! < start of macros which become actual function bodies after expansion >
DeclarationMacro(ArrayCreateByFullPtr)

!! < end of automatically generated functions >
!------------------------------------------------------------------------------

!------------------------------------------------------------------------------
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayCreateByList"
!BOP
! !IROUTINE: ESMF_ArrayCreate -- Create an Array specifying all options.
!
! !INTERFACE:
      ! Private name; call using ESMF_ArrayCreate()
      function ESMF_ArrayCreateByList(rank, type, kind, counts, &
                                      haloWidth, lbounds, ubounds, rc)
!
! !RETURN VALUE:
      type(ESMF_Array) :: ESMF_ArrayCreateByList
!
! !ARGUMENTS:
      integer, intent(in) :: rank
      type(ESMF_DataType), intent(in) :: type
      type(ESMF_DataKind), intent(in) :: kind
      integer, dimension(:), intent(in) :: counts
      integer, intent(in), optional :: haloWidth 
      integer, dimension(:), intent(in), optional :: lbounds
      integer, dimension(:), intent(in), optional :: ubounds
      integer, intent(out), optional :: rc 
!
! !DESCRIPTION:
!  Create a new {\tt ESMF\_Array} and allocate data space, which 
!  remains uninitialized.  The return value is the new {\tt ESMF\_Array}.
!    
!  The arguments are:
!  \begin{description}
!  \item[rank]  
!    Array rank (dimensionality -- 1D, 2D, etc).  Maximum allowed is 7D.
!  \item[type]  
!    Array type.  Valid types include {\tt ESMF\_DATA\_INTEGER},
!    {\tt ESMF\_DATA\_REAL}, {\tt ESMF\_DATA\_LOGICAL}, or
!    {\tt ESMF\_DATA\_CHARACTER}.
!  \item[kind]  
!    Array kind.  Valid kinds include {\tt ESMF\_I4}, 
!    {\tt ESMF\_I8}, {\tt ESMF\_R4}, {\tt ESMF\_R8}.
!    %%% {\tt ESMF\_C8}, {\tt ESMF\_C16}. % add back when supported
!  \item[counts]  
!    The number of items in each dimension of the array.  This is a 1D
!    integer array the same length as the {\tt rank}.
!  \item[{[haloWidth]}]  
!    Set the maximum width of the halo region on all edges. Defaults to 0.
!  \item[{[lbounds]}]  
!    An integer array of length {\tt rank} with the lower index 
!    for each dimension.
!  \item[{[ubounds]}]  
!    An integer array of length {\tt rank} with the upper index 
!    for each dimension.
!   \item[{[rc]}]   
!    Return code; equals {\tt ESMF\_SUCCESS} if there are no errors.
!   \end{description}
!
!EOP

        ! Local vars
        type (ESMF_Array) :: array          ! new C++ Array
        integer :: hwidth                   ! local copy of halo width 
        integer, dimension(ESMF_MAXDIM) :: lb, ub  ! local bounds
        integer :: status                   ! local error status
        logical :: rcpresent                ! did user specify rc?

        status = ESMF_FAILURE
        rcpresent = .FALSE.
        array%this = ESMF_NULL_POINTER

        ! Initialize return code; assume failure until success is certain
        if (present(rc)) then
          rcpresent = .TRUE.
          rc = ESMF_FAILURE
        endif

        ! Always supply a halo value, setting it to 0 if not specified. 
        if (present(haloWidth)) then 
          hwidth = haloWidth 
        else 
          hwidth = 0 
        endif 
        ! Assume defaults first, then alter if lb or ub specified. 
        lb = 1 
        ub(1:size(counts)) = counts 
        if (present(lbounds)) then 
            lb(1:size(lbounds)) = lbounds 
        endif 
        if (present(ubounds)) then 
            ub(1:size(ubounds)) = ubounds 
        endif 

 
        ! TODO: should this take the counts, or not?  for now i am going to
        !  set the counts after i have created the f90 array and not here.
        call c_ESMC_ArrayCreateNoData(array, rank, type, kind, &
                                            ESMF_FROM_FORTRAN, status)
        if (ESMF_LogMsgFoundError(status, &
                                    ESMF_ERR_PASSTHRU, &
                                    ESMF_CONTEXT, rc)) return

        call ESMF_ArrayConstructF90Ptr(array, counts, hwidth, rank, type, &
                                       kind, lb, ub, status)

        ! Set return values
        ESMF_ArrayCreateByList = array 
        if (rcpresent) rc = status

        end function ESMF_ArrayCreateByList


!------------------------------------------------------------------------------
!------------------------------------------------------------------------------

!! < start of macros which become actual function bodies after expansion >
DeclarationMacro(ArrayCreateByMTPtr)

!! < end of automatically generated functions >
!------------------------------------------------------------------------------

!------------------------------------------------------------------------------
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayCreateBySpec"
!BOP
! !IROUTINE: ESMF_ArrayCreate -- Create a new Array from an ArraySpec
!
! !INTERFACE:
      ! Private name; call using ESMF_ArrayCreate()
      function ESMF_ArrayCreateBySpec(arrayspec, counts, haloWidth, &
                                      lbounds, ubounds, rc)
!
! !RETURN VALUE:
      type(ESMF_Array) :: ESMF_ArrayCreateBySpec
!
! !ARGUMENTS:
      type(ESMF_ArraySpec), intent(in) :: arrayspec
      integer, intent(in), dimension(:) :: counts
      integer, intent(in), optional :: haloWidth 
      integer, dimension(:), intent(in), optional :: lbounds
      integer, dimension(:), intent(in), optional :: ubounds
      integer, intent(out), optional :: rc 
!
! !DESCRIPTION:
!  Create a new {\tt ESMF\_Array} and allocate data space, which 
!  remains uninitialized. The return value is the new {\tt ESMF\_Array}.
!    
!  The arguments are:
!  \begin{description}
!  \item[arrayspec]  
!    An {\tt ESMF\_ArraySpec} object which contains the type/kind/rank
!    information for the data.
!  \item[counts]  
!    The number of items in each dimension of the array.  This is a 1D
!    integer array the same length as the {\tt rank}.
!  \item[{[haloWidth]}]  
!    Set the maximum width of the halo region on all edges. Defaults to 0.
!  \item[{[lbounds]}]  
!    An integer array of length {\tt rank} with the lower index 
!    for each dimension.
!  \item[{[ubounds]}]  
!    An integer array of length {\tt rank}, with the upper index 
!    for each dimension.
!   \item[{[rc]}]   
!    Return code; equals {\tt ESMF\_SUCCESS} if there are no errors.
!   \end{description}
!
!EOP

        ! Local vars
        type (ESMF_Array) :: array          ! new C++ Array
        integer :: status                   ! local error status
        logical :: rcpresent                ! did user specify rc?
        integer :: rank
        type(ESMF_DataType) :: type
        type(ESMF_DataKind) :: kind

        status = ESMF_FAILURE
        rcpresent = .FALSE.
        array%this = ESMF_NULL_POINTER

        ! Initialize return code; assume failure until success is certain
        if (present(rc)) then
          rcpresent = .TRUE.
          rc = ESMF_FAILURE
        endif

        call ESMF_ArraySpecGet(arrayspec, rank, type, kind, status)
        if (status .ne. ESMF_SUCCESS) return
        
        ! Call the list function to make the array
        ESMF_ArrayCreateBySpec = ESMF_ArrayCreateByList(rank, type, kind, &
                                                       counts, haloWidth, &
                                                       lbounds, ubounds, status)
        if (rcpresent) rc = status

        end function ESMF_ArrayCreateBySpec


!------------------------------------------------------------------------------

!! < start of macros which become actual function bodies after expansion >
DeclarationMacro(ArrayDeallocate)

!! < end of automatically generated functions >

!------------------------------------------------------------------------------
!------------------------------------------------------------------------------

!------------------------------------------------------------------------------
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayDestroy"
!BOP
! !INTERFACE:
      subroutine ESMF_ArrayDestroy(array, rc)
!
! !ARGUMENTS:
      type(ESMF_Array) :: array
      integer, intent(out), optional :: rc
!
! !DESCRIPTION:
!     Releases all resources associated with this {\tt ESMF\_Array}.
!
!     The arguments are:
!     \begin{description}
!     \item[array]
!       Destroy contents of this {\tt ESMF\_Array}.
!     \item[[rc]]
!       Return code; equals {\tt ESMF\_SUCCESS} if there are no errors.
!     \end{description}
!
!  To reduce the depth of crossings of the F90/C++ boundary we first
!   query to see if we are responsible for deleting the data space.  If so,
!   first deallocate the space and then call the C++ code to release
!   the object space.  When it returns we are done and can return to the user.
!   Otherwise we would need to make a nested call back into F90 from C++ to do
!   the deallocation during the object delete.
!
!EOP

        ! Local vars
        integer :: status                   ! local error status
        logical :: rcpresent                ! did user specify rc?
        logical :: needsdealloc             ! do we need to free space?
        integer :: rank
        type(ESMF_DataType) :: type
        type(ESMF_DataKind) :: kind

        ! Initialize return code; assume failure until success is certain
        status = ESMF_FAILURE
        rcpresent = .FALSE.
        if (present(rc)) then
          rcpresent = .TRUE.
          rc = ESMF_FAILURE
        endif

        ! Simple validity check 
        if (array%this .eq. ESMF_NULL_POINTER) then
          if (ESMF_LogFoundError(ESMF_RC_OBJ_BAD, &
                                 ESMF_CONTEXT, rc)) return
        endif

        needsdealloc = .FALSE.

        ! TODO: document the current rule - if we do the allocate in
        !   the case of ESMF_DATA_COPY at create time then we delete the
        !   space.  otherwise, the user needs to destroy the array 
        !   (we will ignore the data) and call deallocate themselves.

        ! Call Destruct first, then free this memory
        call c_ESMC_ArrayNeedsDealloc(array, needsdealloc, status)
        if (needsdealloc) then
          call c_ESMC_ArrayGetRank(array, rank, status)
          call c_ESMC_ArrayGetType(array, type, status)
          call c_ESMC_ArrayGetKind(array, kind, status)
          call ESMF_ArrayF90Deallocate(array, rank, type, kind, status)
          if (ESMF_LogMsgFoundAllocError(status, "Array deallocate", &
                                         ESMF_CONTEXT, rc)) return

          call c_ESMC_ArraySetNoDealloc(array, status)
        endif

        ! Calling deallocate first means this will not return back to F90
        !  before returning for good.
        call c_ESMC_ArrayDestroy(array, status)
        if (ESMF_LogMsgFoundError(status, &
                                    ESMF_ERR_PASSTHRU, &
                                    ESMF_CONTEXT, rc)) return

        ! mark this as destroyed 
        array%this = ESMF_NULL_POINTER

        ! set return code if user specified it
        if (rcpresent) rc = ESMF_SUCCESS

        end subroutine ESMF_ArrayDestroy


!------------------------------------------------------------------------------
!------------------------------------------------------------------------------
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayConstructF90Ptr"
!BOPI
! !IROUTINE: ESMF_ArrayConstructF90Ptr - Create and add a Fortran ptr to array
!
! !INTERFACE:
     subroutine ESMF_ArrayConstructF90Ptr(array, counts, hwidth, &
                                         rank, type, kind, lbounds, ubounds, rc)
!
! !ARGUMENTS:
      type(ESMF_Array), intent(inout) :: array
      integer, dimension(:), intent(in) :: counts
      integer, intent(in) :: hwidth
      integer, intent(in) :: rank
      type(ESMF_DataType), intent(in) :: type
      type(ESMF_DataKind), intent(in) :: kind
      integer, dimension(:), intent(in) :: lbounds
      integer, dimension(:), intent(in) :: ubounds
      integer, intent(out) :: rc 
!
! !DESCRIPTION:
!  Take a partially created {\tt ESMF\_Array} and type/kind/rank 
!  information and call the proper subroutine to create an F90 pointer, 
!  allocate space, and set the corresponding values in the 
!  {\tt ESMF\_Array} object header.
!    
!  The arguments are:
!  \begin{description}
!  \item[array]
!    Partially created {\tt ESMF\_Array} object.  This entry point is used
!    during both the C++ and F90 create calls if we need to create an F90
!    pointer to be used later.
!  \item[counts]
!    The number of items in each dimension of the array.  This is a 1D
!    integer array the same length as the {\tt rank}.
!  \item[hwidth]
!    The halo width on all edges.  Used to set the computational area
!    in the array.
!  \item[rank]
!    Array rank.
!    This must match what is already in the array - it is here only as
!    a convenience.
!  \item[type]
!    Array type.
!    This must match what is already in the array - it is here only as
!    a convenience.
!  \item[kind]
!    Array kind. 
!    This must match what is already in the array - it is here only as
!    a convenience.
!   \item[lbounds]
!    The lower index values per rank.
!   \item[ubounds]
!    The upper index values per rank.
!   \item[rc]
!    Return code; equals {\tt ESMF\_SUCCESS} if there are no errors.
!   \end{description}
!
!EOP


        ! Local vars
        integer :: localkind, localtype

        ! Initialize return code; assume failure until success is certain
        ! Note from this point down in the calling stack rc is not optional.
        ! This is all internal code, heavily macroized - no reason to add
        ! unnecessary code to check for non-present error return variables.
        rc = ESMF_FAILURE

        localtype = type%dtype
        localkind = kind%dkind

        ! Call a T/K/R specific interface in order to create the proper
        !  type of F90 pointer, allocate the space, set the values in the
        !  Array object, and return.  (The routine this code is calling is
        !  generated by macro.)

        ! Call proper create F90 ptr routine
        select case (localtype)
          case (ESMF_DATA_INTEGER%dtype)
            select case (rank)
              case (1)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr1DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr1DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr1DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr1DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (2)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr2DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr2DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr2DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr2DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (3)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr3DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr3DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr3DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr3DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (4)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr4DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr4DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr4DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr4DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
^ifndef ESMF_NO_GREATER_THAN_4D
              case (5)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr5DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr5DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr5DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr5DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (6)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr6DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr6DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr6DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr6DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (7)
                select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
                  case (ESMF_I1%dkind)
                    call ESMF_ArrayConstructF90Ptr7DI1(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
                  case (ESMF_I2%dkind)
                    call ESMF_ArrayConstructF90Ptr7DI2(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
^endif
                  case (ESMF_I4%dkind)
                    call ESMF_ArrayConstructF90Ptr7DI4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_I8%dkind)
                    call ESMF_ArrayConstructF90Ptr7DI8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
^endif
    
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
            end select
    
           case (ESMF_DATA_REAL%dtype)
            select case (rank)
              case (1)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr1DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr1DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (2)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr2DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr2DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (3)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr3DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr3DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (4)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr4DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr4DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
^ifndef ESMF_NO_GREATER_THAN_4D
              case (5)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr5DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr5DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (6)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr6DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr6DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
    
              case (7)
                select case (localkind)
                  case (ESMF_R4%dkind)
                    call ESMF_ArrayConstructF90Ptr7DR4(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case (ESMF_R8%dkind)
                    call ESMF_ArrayConstructF90Ptr7DR8(array, counts, hwidth, &
                         lbounds=lbounds, ubounds=ubounds, rc=rc)
                  case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
                end select
^endif
    
              case default
                 if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
            end select
          case default
            if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported type", &
                                 ESMF_CONTEXT, rc)) return
         end select

        ! Note: rc is already set, nothing to do here.

        end subroutine ESMF_ArrayConstructF90Ptr

!------------------------------------------------------------------------------

!! < start of macros which become actual function bodies after expansion >
DeclarationMacro(ArrayConstructF90Ptr)

!! < end of automatically generated functions >
!------------------------------------------------------------------------------
!------------------------------------------------------------------------------
!------------------------------------------------------------------------------
!
! This section is Allocate/Deallocate for Arrays
!
!------------------------------------------------------------------------------

!------------------------------------------------------------------------------ 
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayF90Allocate"
!BOPI
! !IROUTINE:  ESMF_ArrayF90Allocate - Allocate an F90 pointer and set Array info
!
! !INTERFACE: 
     subroutine ESMF_ArrayF90Allocate(array, rank, type, kind, &
                                      counts, lbounds, ubounds, hwidth, rc)
! 
! !ARGUMENTS: 
      type(ESMF_Array), intent(inout) :: array 
      integer, intent(in) :: rank   
      type(ESMF_DataType), intent(in) :: type
      type(ESMF_DataKind), intent(in) :: kind
      integer, dimension(:), intent(in) :: counts 
      integer, dimension(:), intent(in) :: lbounds 
      integer, dimension(:), intent(in) :: ubounds 
      integer, intent(in) :: hwidth 
      integer, intent(out), optional :: rc 
! 
! !DESCRIPTION: 
!     Allocate data contents for an {\tt ESMF\_Array} created 
!     from the C++ interface. 
!     The arguments are: 
!     \begin{description} 
!     \item[array]  
!          A partially created {\tt ESMF\_Array} object. 
!     \item[rank]  
!          The {\tt ESMF\_Array} rank.  
!     \item[type]  
!          The {\tt ESMF\_Array} type (integer, real/float, etc).  
!     \item[kind]  
!          The {\tt ESMF\_Array} kind (short/2, long/8, etc).  
!     \item[counts]  
!          An integer array, size {\tt rank}, of each dimension length. 
!     \item[lbounds]  
!          An integer array, size {\tt rank}, of each dimension lower index.
!     \item[ubounds]  
!          An integer array, size {\tt rank}, of each dimension upper index.
!     \item[hwidth]  
!          An integer halo width, a single value, applied to each dimension. 
!     \item[{[rc]}]  
!          Return code; equals {\tt ESMF\_SUCCESS} if there are no errors. 
!   \end{description} 
! 
!EOPI
! !REQUIREMENTS: 
 
    integer :: status                               ! local error status 
    integer, dimension(ESMF_MAXDIM) :: lb, ub
    integer, dimension(ESMF_MAXDIM) :: offsets
    integer :: localkind, localtype

    !! local variables, expanded by macro
AllTypesMacro(ArrayLocalVar)
 
    status = ESMF_FAILURE  
    if (present(rc)) rc = ESMF_FAILURE
 
    lb(1:size(lbounds)) = lbounds
    ub(1:size(ubounds)) = ubounds

    localtype = type%dtype
    localkind = kind%dkind

    !! macros which are expanded by the preprocessor
    select case (localtype)
      case (ESMF_DATA_INTEGER%dtype)
        select case (rank)
          case (1)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 1, RNG1, LOC1)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 1, RNG1, LOC1)
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 1, RNG1, LOC1)
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 1, RNG1, LOC1)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (2)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 2, RNG2, LOC2)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 2, RNG2, LOC2)
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 2, RNG2, LOC2)
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 2, RNG2, LOC2)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (3)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 3, RNG3, LOC3)       
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 3, RNG3, LOC3)       
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 3, RNG3, LOC3)       
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 3, RNG3, LOC3)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (4)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 4, RNG4, LOC4)       
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 4, RNG4, LOC4)       
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 4, RNG4, LOC4)       
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 4, RNG4, LOC4)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

^ifndef ESMF_NO_GREATER_THAN_4D
          case (5)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 5, RNG5, LOC5)       
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 5, RNG5, LOC5)       
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 5, RNG5, LOC5)       
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 5, RNG5, LOC5)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (6)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 6, RNG6, LOC6)       
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 6, RNG6, LOC6)       
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 6, RNG6, LOC6)       
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 6, RNG6, LOC6)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (7)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocAllocateMacro(I1, 7, RNG7, LOC7)       
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocAllocateMacro(I2, 7, RNG7, LOC7)       
^endif
              case (ESMF_I4%dkind)
AllocAllocateMacro(I4, 7, RNG7, LOC7)       
              case (ESMF_I8%dkind)
AllocAllocateMacro(I8, 7, RNG7, LOC7)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select
^endif

          case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
        end select

       case (ESMF_DATA_REAL%dtype)
        select case (rank)
          case (1)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 1, RNG1, LOC1)
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 1, RNG1, LOC1)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (2)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 2, RNG2, LOC2)
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 2, RNG2, LOC2)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (3)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 3, RNG3, LOC3)       
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 3, RNG3, LOC3)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (4)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 4, RNG4, LOC4)       
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 4, RNG4, LOC4)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

^ifndef ESMF_NO_GREATER_THAN_4D
          case (5)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 5, RNG5, LOC5)       
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 5, RNG5, LOC5)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (6)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 6, RNG6, LOC6)       
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 6, RNG6, LOC6)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (7)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocAllocateMacro(R4, 7, RNG7, LOC7)       
              case (ESMF_R8%dkind)
AllocAllocateMacro(R8, 7, RNG7, LOC7)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select
^endif

          case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
        end select
      case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported type", &
                                 ESMF_CONTEXT, rc)) return
     end select

     if (present(rc)) rc = status 
 
     end subroutine ESMF_ArrayF90Allocate
 

!------------------------------------------------------------------------------ 
^undef  ESMF_METHOD
^define ESMF_METHOD "ESMF_ArrayF90Deallocate"
!BOPI
! !IROUTINE:  ESMF_ArrayF90Deallocate - Deallocate an F90 pointer 
!
! !INTERFACE: 
     subroutine ESMF_ArrayF90Deallocate(array, rank, type, kind, rc)
! 
! !ARGUMENTS: 
      type(ESMF_Array) :: array 
      integer :: rank   
      type(ESMF_DataType) :: type
      type(ESMF_DataKind) :: kind
      integer, intent(out), optional :: rc 
! 
! !DESCRIPTION: 
!     Deallocate data contents for an {\tt ESMF\_Array} created 
!     from the C++ interface. 
!     The arguments are: 
!     \begin{description} 
!     \item[array]  
!          A partially created {\tt ESMF\_Array} object. 
!     \item[rank]  
!          The {\tt ESMF\_Array} rank.  
!     \item[type]  
!          The {\tt ESMF\_Array} type (integer, real/float, etc).  
!     \item[kind]  
!          The {\tt ESMF\_Array} kind (short/2, long/8, etc).  
!     \item[{[rc]}]  
!          Return code; equals {\tt ESMF\_SUCCESS} if there are no errors. 
!   \end{description} 
! 
!EOPI
 
    integer :: status                               ! local error status 
    integer :: localkind, localtype

    !! local variables, expanded by macro
AllTypesMacro(ArrayLocalVar)


    if (present(rc)) rc = ESMF_FAILURE
 
    localtype = type
    localkind = kind


    !! macros which are expanded by the preprocessor
    select case (localtype)
      case (ESMF_DATA_INTEGER%dtype)
        select case (rank)
          case (1)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 1)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 1)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 1)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 1)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (2)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 2)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 2)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 2)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 2)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (3)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 3)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 3)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 3)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 3)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (4)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 4)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 4)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 4)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 4)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

^ifndef ESMF_NO_GREATER_THAN_4D
          case (5)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 5)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 5)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 5)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 5)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (6)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 6)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 6)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 6)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 6)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (7)
            select case (localkind)
^ifndef ESMF_NO_INTEGER_1_BYTE
              case (ESMF_I1%dkind)
AllocDeallocateMacro(I1, 7)
^endif
^ifndef ESMF_NO_INTEGER_2_BYTE
              case (ESMF_I2%dkind)
AllocDeallocateMacro(I2, 7)
^endif
              case (ESMF_I4%dkind)
AllocDeallocateMacro(I4, 7)
              case (ESMF_I8%dkind)
AllocDeallocateMacro(I8, 7)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select
^endif

          case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
        end select

       case (ESMF_DATA_REAL%dtype)
        select case (rank)
          case (1)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 1)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 1)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (2)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 2)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 2)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (3)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 3)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 3)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (4)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 4)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 4)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

^ifndef ESMF_NO_GREATER_THAN_4D
          case (5)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 5)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 5)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (6)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 6)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 6)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select

          case (7)
            select case (localkind)
              case (ESMF_R4%dkind)
AllocDeallocateMacro(R4, 7)
              case (ESMF_R8%dkind)
AllocDeallocateMacro(R8, 7)
              case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported kind", &
                                 ESMF_CONTEXT, rc)) return
            end select
^endif

          case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported rank", &
                                 ESMF_CONTEXT, rc)) return
        end select
      case default
                    if (ESMF_LogMsgFoundError(ESMF_RC_ARG_BAD, &
                                "Unsupported type", &
                                 ESMF_CONTEXT, rc)) return
     end select


      if (ESMF_LogMsgFoundAllocError(status, "Array Deallocation", &
                                       ESMF_CONTEXT, rc)) return

     if (present(rc)) rc = ESMF_SUCCESS
 
     end subroutine ESMF_ArrayF90Deallocate
 
!------------------------------------------------------------------------------ 


        end module ESMF_ArrayCreateMod

