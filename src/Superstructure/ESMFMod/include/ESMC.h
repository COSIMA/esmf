// $Id: ESMC.h,v 1.6.6.1 2006/11/16 00:15:55 cdeluca Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2008, University Corporation for Atmospheric Research,
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics
// Laboratory, University of Michigan, National Centers for Environmental
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory,
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.
//

// main include file which includes all others

// these lines prevent this file from being read more than once if it
// ends up being included multiple times

#ifndef ESMC_H
#define ESMC_H


// common macros and constants, #defines
#include "ESMC_Start.h"

// base class, I/O, and error
#include "ESMC_Base.h"
#include "ESMC_IOSpec.h"
#include "ESMC_LogErr.h"

// time manager
#include "ESMC_Fraction.h"
#include "ESMC_BaseTime.h"
#include "ESMC_Calendar.h"
#include "ESMC_TimeInterval.h"
#include "ESMC_Time.h"
#include "ESMC_Alarm.h"
#include "ESMC_Clock.h"

// local array, datamap
#include "ESMC_ArraySpec.h"
#include "ESMC_LocalArray.h"
#include "ESMC_ArrayDataMap.h"

// vm and layout
#include "ESMC_VMKernel.h"
#include "ESMC_VM.h"
#include "ESMC_DELayout.h"
// #include "ESMC_Machine.h"
// #include "ESMC_PE.h"
// #include "ESMC_PEList.h"
// #include "ESMC_DELayout.h"
// #include "ESMC_CommMem.h"

// array, grid, field, bundle, comms
#include "ESMC_Array.h"
#include "ESMC_Grid.h"
//#include "ESMC_FieldDataMap.h"
#include "ESMC_Field.h"
//#include "ESMC_BundleDataMap.h"
#include "ESMC_Bundle.h"
#include "ESMC_ArrayComm.h"

// components and related items
#include "ESMC_Xform.h"
#include "ESMC_State.h"
#include "ESMC_FTable.h"
#include "ESMC_Comp.h"
#include "ESMC_GridComp.h"
#include "ESMC_CplComp.h"

// framework-wide initialization and finalization
#include "ESMC_Init.h"


#endif  // ESMC_H

