#if 0
! $Id: ESMF_FieldGetMacros.h,v 1.5.6.1 2006/11/16 00:15:28 cdeluca Exp $
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
! Macros for the Field class.
!------------------------------------------------------------------------------
#endif

#include "ESMF_StdCppMacros.h"

#if 0
!------------------------------------------------------------------------------
! Documentation for the general FieldGetDataPointer<> macros.
!------------------------------------------------------------------------------
#endif

#define FieldGetDataPointerDoc() \
!------------------------------------------------------------------------------ @\
! <Created by macro - do not edit directly > @\
!BOP @\
! !IROUTINE: ESMF_FieldGetDataPointer - Retrieve Fortran pointer directly from a Field @\
! @\
! !INTERFACE: @\
!      ! Private name; call using ESMF_FieldGetDataPointer() @\
!      subroutine ESMF_FieldGetDataPointer<rank><type><kind>(field, ptr, copyflag, rc) @\
! @\
! !ARGUMENTS: @\
!      type(ESMF_Field), intent(in) :: field @\
!      <type> (ESMF_KIND_<kind>), dimension(<rank>), pointer :: ptr @\
!      type(ESMF_CopyFlag), intent(in), optional :: copyflag @\
!      integer, intent(out), optional :: rc   @\
! @\
! !DESCRIPTION: @\
! Returns a direct Fortran pointer to the data in an {\tt ESMF\_Field}. @\
! Valid type/kind/rank combinations supported by the @\
! framework are: ranks 1 to 7, type real of kind *4 or *8, @\
! and type integer of kind *1, *2, *4, or *8. @\
! @\
! The arguments are: @\
!  \begin{description} @\
!  \item[field] @\
!   The {\tt ESMF\_Field} to query. @\
!  \item[ptr] @\
!   An unassociated Fortran pointer of the proper Type, Kind, and Rank as @\
!   the data in the Field.  When this call returns successfully, the pointer @\
!   will now point to the data in the Field.  This is either a reference or @\
!   a copy, depending on the setting of the following argument.  @\
!  \item[{[copyflag]}] @\
!   Defaults to {\tt ESMF\_DATA\_REF}.  If set to {\tt ESMF\_DATA\_COPY}, @\
!   a separate copy of the data will be allocated and the pointer will point @\
!   at the copy.  If a new copy of the data is made, the caller is @\
!   responsible for deallocating the space. @\
!  \item[{[rc]}] @\
!    Return code; equals {\tt ESMF\_SUCCESS} if there are no errors. @\
!  \end{description} @\
! @\
!EOP @\

#if 0
!------------------------------------------------------------------------------
! Create a new array based on an unallocated Fortran array and a list of counts.
!------------------------------------------------------------------------------
#endif

#define FieldGetDataPointerMacro(mname, mtypekind, mrank, mdim, mlen, mrng, mloc) \
!------------------------------------------------------------------------------ @\
! <Created by macro - do not edit directly > @\
^undef  ESMF_METHOD @\
^define ESMF_METHOD "ESMF_FieldGetDataPointer" @\
      subroutine ESMF_FieldGetDataPointer##mrank##D##mtypekind(field, ptr, copyflag, rc) @\
 @\
      type(ESMF_Field), intent(in) :: field @\
      mname (ESMF_KIND_##mtypekind), dimension(mdim), pointer :: ptr @\
      type(ESMF_CopyFlag), intent(in), optional :: copyflag @\
      integer, intent(out), optional :: rc   @\
@\
        ! Local variables @\
        type (ESMF_Array) :: array          ! array object @\
        integer :: status                   ! local error status @\
        logical :: rcpresent                ! did user specify rc? @\
 @\
        ! Initialize return code; assume failure until success is certain @\
        status = ESMF_FAILURE @\
        rcpresent = .FALSE. @\
        array%this = ESMF_NULL_POINTER @\
 @\
        if (present(rc)) then @\
          rcpresent = .TRUE. @\
          rc = ESMF_FAILURE @\
        endif @\
 @\
        ! Test to see if pointer already associated, and fail if so. @\
        ! TODO: check this - this test seems to always be true, even if @\
        !  the pointer has been nullified first? @\
        !if (associated(ptr)) then @\
        !  if (ESMF_LogMsgFoundError(ESMF_RC_OBJ_BAD, & @\
        !                      "Data Pointer cannot already be associated", & @\
        !                      ESMF_CONTEXT, rc)) return @\
        !endif @\
 @\
        call ESMF_FieldGetArray(field, array, rc=status) @\
        if (ESMF_LogMsgFoundError(status, & @\
                                  ESMF_ERR_PASSTHRU, & @\
                                  ESMF_CONTEXT, rc)) return @\
 @\
        call ESMF_ArrayGetData(array, ptr, copyflag, rc=status) @\
        if (ESMF_LogMsgFoundError(status, & @\
                                  ESMF_ERR_PASSTHRU, & @\
                                  ESMF_CONTEXT, rc)) return @\
 @\
        if (rcpresent) rc = status @\
 @\
        end subroutine ESMF_FieldGetDataPointer##mrank##D##mtypekind  @\
 @\
! < end macro - do not edit directly >  @\
!------------------------------------------------------------------------------ @\

