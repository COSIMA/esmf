// $Id: ESMC_Route.h,v 1.38.2.1 2005/04/28 21:04:35 jwolfe Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2003, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the GPL.

// ESMF Route C++ declaration include file
//
// (all lines below between the !BOP and !EOP markers will be included in 
//  the automated document processing.)
//-----------------------------------------------------------------------------
//
 // these lines prevent this file from being read more than once if it
 // ends up being included multiple times

 #ifndef ESMC_Route_H
 #define ESMC_Route_H

//-----------------------------------------------------------------------------

 // Put any constants or macros which apply to the whole component in this file.
 // Anything public or esmf-wide should be up higher at the top level
 // include files.

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  ESMC_Route - Top level Route object
//
// !DESCRIPTION:
//
// The code in this file defines the C++ Route members and declares method 
// signatures (prototypes).  The companion file ESMC_Route.C contains
// the definitions (full code bodies) for the Route methods.
//
// 
//
//-----------------------------------------------------------------------------
// 
// !USES:
 #include <ESMC_Base.h>        // all classes inherit from the ESMC Base class.
 #include <ESMC_DELayout.h>
 #include <ESMC_XPacket.h>
 #include <ESMC_RTable.h>
 #include <ESMC_CommTable.h>

// !PUBLIC TYPES:
 class ESMC_Route;

// !PRIVATE TYPES:

 // structure for matching precomputed routes
 typedef struct {
    int entrystatus;
    int rank;
    ESMC_DELayout *snd_delayout;
    int snd_DE; 
    int snd_AI_count; 
    ESMC_AxisIndex *snd_AI_exc;
    ESMC_AxisIndex *snd_AI_tot;
    ESMC_DELayout *rcv_delayout;
    int rcv_DE; 
    int rcv_AI_count; 
    ESMC_AxisIndex *rcv_AI_exc;
    ESMC_AxisIndex *rcv_AI_tot;
    ESMC_Logical periodic[ESMF_MAXDIM];
    int routeid;
    ESMC_Route *theroute;
 } ESMC_RouteCacheEntry;

 // structure for caching them
 typedef struct {
    int nroutes;
    int nalloc;
    ESMC_RouteCacheEntry **rcep;
 } ESMC_RouteCacheTable;

 // class declaration type
 class ESMC_Route : public ESMC_Base {    // inherits from ESMC_Base class

   private:
     // name in base class
     int routeid;           // unique id, used later for cacheing
     ESMC_DELayout *delayout; // layout which includes all src + dst de's
     ESMC_RTable *sendRT;   // send route table
     ESMC_RTable *recvRT;   // receive route table
     int recvitems;         // if >0, numitems needed in the destination array
     ESMC_CommTable *ct;    // communication scheduling table

// !PUBLIC MEMBER FUNCTIONS:

  public:
 // ESMC_RouteCreate and ESMC_RouteDestroy are declared below,
 // outside the ESMC_Route declaration
    int ESMC_RouteConstruct(ESMC_DELayout *delayout);
    int ESMC_RouteDestruct(void);

 // accessor methods for class members
    //int ESMC_RouteGet(<value type> *value) const;

    int ESMC_RouteSetSend(int dst_de, ESMC_XPacket *xp);
    int ESMC_RouteSetRecv(int src_de, ESMC_XPacket *xp);
    
    int ESMC_RouteSetRecvItems(int nitems);
    int ESMC_RouteGetRecvItems(void);

    // initialize the communication routines in this route object
    int ESMC_RoutePrecomputeHalo(int rank, int my_DE, ESMC_AxisIndex *AI_exc,
                       ESMC_AxisIndex *AI_tot, int AI_count, 
                       int *global_start, int *global_count,
                       ESMC_DELayout *delayout,
                       ESMC_Logical *periodic = NULL);
    int ESMC_RoutePrecomputeRedist(int rank, int dstMyDE,
                       ESMC_AxisIndex *dstCompAI, ESMC_AxisIndex *dstTotalAI,
                       int dstAICount, int *dstGlobalStart, int *dstGlobalCount,
                       ESMC_DELayout *dstdeLayout, int srcMyDE, 
                       ESMC_AxisIndex *srcCompAI, ESMC_AxisIndex *srcTotalAI,
                       int srcAICount, int *srcGlobalStart, int *srcGlobalCount,
                       ESMC_DELayout *srcdeLayout);
    int ESMC_RoutePrecomputeRegrid(int rank, int my_DE_rcv, 
                       ESMC_AxisIndex *AI_rcv_exc, ESMC_AxisIndex *AI_rcv_tot,
                       int AI_rcv_count, int *global_start_rcv,
                       int *global_count_rcv, ESMC_DELayout *delayout_rcv,
                       int my_DE_snd, 
                       ESMC_AxisIndex *AI_snd_exc, ESMC_AxisIndex *AI_snd_tot,
                       int AI_snd_count, int *global_start_snd,
                       int *global_count_snd, ESMC_DELayout *delayout_snd);
    int ESMC_RoutePrecomputeDomList(int rank, ESMC_DELayout *srcDELayout,
                       ESMC_DELayout *dstDELayout,
                       ESMC_DomainList *srcDomainList,
                       ESMC_DomainList *dstDomainList,
                       ESMC_Logical *hasSrcData, ESMC_Logical *hasDstData);

    // execute the communication routines set up in this route object
    int ESMC_RouteRun(void *srcaddr, void *dstaddr, ESMC_DataKind dk);

    // add a route to the cache table
    int ESMC_RouteAddCache(int rank, 
                       int my_DE_rcv, 
                       ESMC_AxisIndex *AI_rcv_exc, ESMC_AxisIndex *AI_rcv_tot,
                       int AI_rcv_count, ESMC_DELayout *delayout_rcv,
                       int my_DE_snd, 
                       ESMC_AxisIndex *AI_snd_exc, ESMC_AxisIndex *AI_snd_tot,
                       int AI_snd_count, ESMC_DELayout *delayout_snd,
                       ESMC_Logical *periodic);

    // drop a route from the cache table
    int ESMC_RouteDropCache(void);

 // required methods inherited and overridden from the ESMC_Base class
    int ESMC_RouteValidate(const char *options) const;
    int ESMC_RoutePrint(const char *options) const;

 // native C++ constructors/destructors
	ESMC_Route(void);
	~ESMC_Route(void);
  
// !PRIVATE MEMBER FUNCTIONS:
//
  private: 

//EOP
//-----------------------------------------------------------------------------

 };   // end class ESMC_Route

// Create and Destroy are declared as class helper functions (not methods)
// since they create and destroy an ESMC_Route object itself. E.g. if Create
// were a method, the ESMC_Route object on whose behalf it was being invoked
// would need to already exist!  These functions are supersets of C++ new
// and delete; they perform allocation/deallocation specialized to
// an ESMC_Route object.

 ESMC_Route *ESMC_RouteCreate( ESMC_DELayout *delayout, int *rc);
 int ESMC_RouteDestroy(ESMC_Route *route);
 int ESMC_RouteGetCached(int rank, 
                       int my_DE_rcv, 
                       ESMC_AxisIndex *AI_rcv_exc, ESMC_AxisIndex *AI_rcv_tot,
                       int AI_rcv_count, ESMC_DELayout *delayout_rcv,
                       int my_DE_snd, 
                       ESMC_AxisIndex *AI_snd_exc, ESMC_AxisIndex *AI_snd_tot,
                       int AI_snd_count, ESMC_DELayout *delayout_snd,
                       ESMC_Logical *periodic,
                       ESMC_Logical *hascachedroute, ESMC_Route **route);

 #endif  // ESMC_Route_H
