// $Id: ESMC_DELayout.h,v 1.19.4.2 2006/11/16 06:14:39 cdeluca Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2006, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the University of Illinois-NCSA License.

// ESMC DELayout include file for C++

// (all lines below between the !BOP and !EOP markers will be included in
//  the automated document processing.)
//-------------------------------------------------------------------------
// these lines prevent this file from being read more than once if it
// ends up being included multiple times

#ifndef ESMC_DELayout_H
#define ESMC_DELayout_H

//-------------------------------------------------------------------------
//BOP
// !CLASS: ESMC_DELayout - DELayout
//
// !DESCRIPTION:
//
// The code in this file defines the C++ {\tt DELayout} members and method
// signatures (prototypes).  The companion file {\tt ESMC\_DELayout.C}
// contains the full code (bodies) for the {\tt DELayout} methods.
//
///EOP
//-------------------------------------------------------------------------

#include <ESMC_Base.h>  
#include <ESMC_VM.h>  

// normal connection weight
const int ESMC_CWGHT_NORMAL = 50;

// parameters for asynchronous/sync communication 
// keep in sync w/ fortran declaration
enum ESMC_Complete {ESMC_TEST_COMPLETE=1, ESMC_WAIT_COMPLETE};

// keep in sync w/ fortran declaration
class ESMC_CommHandle {
  int mpi_handle;
  enum ESMC_Complete cflag;
};



class ESMC_DELayout;


// DE type used internally in the ESMC_DELayout class
typedef struct{
  int deid;         // DE's external id number (in case not base zero)
  int petid;        // Id of the PET associated with this DE
  int pid;          // absolute process ID, specifying virtual memory space
  int nconnect;     // number of connections from this DE
  int *connect_de;  // connected DEs 
  int *connect_w;   // connection weight
  int *coord;       // coordinates of this DE in the layout
}de_type;
  

// class definition
class ESMC_DELayout : public ESMC_Base {    // inherits from ESMC_Base class
  private:
    ESMC_VM *myvm;  // ptr to this PET's VM instance this layout is running on
    int ndes;       // number of DEs
    de_type *des;   // list that holds all of this layout's DE info
    int nmydes;     // number of DEs associated with instantiating PET
    int *mydes;     // list that holds all of the des indices for this instance
    int ndim;       // dimensionality of this layout
    ESMC_Logical oneToOneFlag;  // indicate whether this is a 1-to-1 layout
    ESMC_Logical logRectFlag;   // indicate whether this is logical rectangular
    int *dims;      // sizes of dimensions in a logical rectangular layout
  public:
    // Construct and Destruct
    int ESMC_DELayoutConstruct1D(ESMC_VM &vm, int nDEs, int *DEtoPET,  
      int len, ESMC_Logical cyclic);
    int ESMC_DELayoutConstructND(ESMC_VM &vm, int *nDEs, int nndim, 
      int *DEtoPET, int len, ESMC_Logical cyclic);
    int ESMC_DELayoutDestruct(void);
    // Get info
    int ESMC_DELayoutGetVM(ESMC_VM **vm);
    int ESMC_DELayoutGet(int *nDEs, int *ndim, int *nmyDEs, int *myDEs, 
      int len_myDEs, int *localDe, ESMC_Logical *oneToOneFlag,
      ESMC_Logical *logRectFlag, int *deCountPerDim, int len_deCountPerDim);
    int ESMC_DELayoutGetDELocalInfo(int DEid, int *DEcoord, int len_coord, 
      int *DEcde, int len_cde, int *DEcw, int len_cw, int *nDEc, int *pid);
    int ESMC_DELayoutGetDEMatchDE(int DEid, ESMC_DELayout &layoutMatch,
      int *deMatchCount, int *deMatchList, int len_deMatchList);
    int ESMC_DELayoutGetDEMatchPET(int DEid, ESMC_VM &vmMatch,
      int *petMatchCount, int *petMatchList, int len_petMatchList);
    // IO and validation
    friend ESMC_DELayout *ESMC_DELayoutDeserialize(char *buffer, int *offset);
    int ESMC_DELayoutPrint(void);
    int ESMC_DELayoutSerialize(char *buffer, int *length, int *offset);
    int ESMC_DELayoutValidate(void);
    // Communication
    int ESMC_DELayoutCopy(void **srcdata, void **destdata, 
      int blen, int srcDE, int destDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutCopy(void **srcdata, void **destdata, 
      int len, ESMC_DataKind dtk, int srcDE, int destDE, 
      ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutExchange(void **srcData1, void **srcData2, 
      void **dstData1, void **dstData2, int blen1, int blen2, int de1, int de2,
      ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutExchange(void **srcData1, void **srcData2, 
      void **dstData1, void **dstData2, int len1, int len2, ESMC_DataKind dtk1,
      ESMC_DataKind dtk2, int de1, int de2, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutBcast(void **data, int blen, int rootDE, 
      ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutBcast(void **data, int len, ESMC_DataKind dtk, 
      int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutScatter(void **srcdata, void **destdata, 
      int blen, int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutScatter(void **srcdata, void **destdata, 
      int len, ESMC_DataKind dtk, int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutGather(void **srcdata, void **destdata, 
      int blen, int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutGather(void **srcdata, void **destdata, 
      int len, ESMC_DataKind dtk, int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutGatherV(void **srcdata, void **destdata, 
      int *blen, int *bdestdispl, int rootDE, ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutGatherV(void **srcdata, void **destdata, 
      int *blen, int *bdestdispl, ESMC_DataKind dtk, int rootDE, 
      ESMC_Logical oneToOneFlag);
    int ESMC_DELayoutAllFullReduce(void **srcdata, void *result, int len,
      ESMC_DataKind dtk, ESMC_Operation op, ESMC_Logical oneToOneFlag);
  private:
    int ESMC_DELayoutFindDEtoPET(int npets);
    int ESMC_DELayoutFillLocal(int mypet);        
};  // end class ESMC_DELayout


// external methods:  

ESMC_DELayout *ESMC_DELayoutCreate(ESMC_VM &vm, int *nDEs, int ndim, 
  int *DEtoPET, int len, ESMC_Logical *cyclic, int *rc);

int ESMC_DELayoutDestroy(ESMC_DELayout **layout);
  
void **ESMC_DELayoutDataCreate(int n, int *rc);
int ESMC_DELayoutDataAdd(void **ptr, void *a, int index);
int ESMC_DELayoutDataDestroy(void **ptr);

ESMC_DELayout *ESMC_DELayoutDeserialize(char *buffer, int *offset);

#endif  // ESMC_DELayout_H

