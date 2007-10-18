// $Id: ESMF_Pthread.h,v 1.1.4.3 2007/10/18 02:43:37 cdeluca Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2007, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//-------------------------------------------------------------------------

// Purpose of this file is to provide a wrapper around the pthread library
// header. This allows to switch from a real pthread library implementation
// to pthread_stubs in a single place.

//-------------------------------------------------------------------------
// these lines prevent this file from being read more than once if it
// ends up being included multiple times

#ifndef ESMF_PTHREAD_H
#define ESMF_PTHREAD_H

#ifdef ESMF_NO_PTHREADS
#include "pthread_stubs.h"    // use pthread_stubs
#else
#include <pthread.h>          // use a real pthread library implementation
#endif

#endif  // ESMF_PTHREAD_H
