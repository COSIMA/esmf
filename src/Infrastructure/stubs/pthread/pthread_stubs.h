// $Id: pthread_stubs.h,v 1.2.4.2 2006/11/16 18:48:40 theurich Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2008, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

//-----------------------------------------------------------------------------
//
// !DESCRIPTION: Provide Pthread stub typedefs and macros to allow compilation
//               of the ESMC_VMKernel in the absence of a Pthread library.
//
//-----------------------------------------------------------------------------

#ifndef _PTHREAD_H
#define _PTHREAD_H	1
#include <sys/types.h>

#ifndef __sgi
#ifndef _AIX
#ifndef __osf__

#ifndef _BITS_PTHREADTYPES_H
#define _BITS_PTHREADTYPES_H	1

typedef struct{} pthread_mutex_t;
typedef struct{} pthread_cond_t;
typedef int pthread_t;

#endif

#endif
#endif
#endif

#define pthread_mutex_init(a,b)
#define pthread_mutex_lock(a) 0;
#define pthread_mutex_unlock(a) 0;
#define pthread_mutex_destroy(a)
#define pthread_self() 0
#define pthread_cond_init(a,b)
#define pthread_cond_wait(a,b)
#define pthread_cond_broadcast(a)
#define pthread_cond_destroy(a)
#define pthread_cond_signal(a)
#define pthread_create(a,b,c,d) 0
#define pthread_join(a,b)

#endif	/* _PTHREAD_H */
