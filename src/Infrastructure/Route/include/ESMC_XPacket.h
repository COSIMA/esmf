// $Id: ESMC_XPacket.h,v 1.27.4.1 2005/03/02 22:09:51 jwolfe Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2003, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the GPL.

// ESMF XPacket C++ declaration include file
//
// (all lines below between the !BOP and !EOP markers will be included in 
//  the automated document processing.)
//-----------------------------------------------------------------------------
//
 // these lines prevent this file from being read more than once if it
 // ends up being included multiple times

 #ifndef ESMC_XPacket_H
 #define ESMC_XPacket_H

//-----------------------------------------------------------------------------

 // Put any constants or macros which apply to the whole component in this file.
 // Anything public or esmf-wide should be up higher at the top level
 // include files.
 #include <ESMC_LocalArray.h>

//-----------------------------------------------------------------------------
//BOP
// !CLASS:  ESMC_XPacket - one line general statement about this class
//
// !DESCRIPTION:
//
// The code in this file defines the C++ XPacket members and declares method 
// signatures (prototypes).  The companion file ESMC_XPacket.C contains
// the definitions (full code bodies) for the XPacket methods.
//
// XPackets are used to give the source and destination locations for
//  calls to Route - they allow scatter/gather behavior, or copy to
//  intermediate contiguous buffers, depending on the machine characteristics.
//
//-----------------------------------------------------------------------------
// 
// !USES:
 #include <ESMC_Base.h>  // all classes inherit from the ESMC Base class.

// !PUBLIC TYPES:
 class ESMC_XPacket;

// !PRIVATE TYPES:

 // class declaration type
 class ESMC_XPacket {    // does *not* inherit from base.

   private:
     // one of these per memory transfer

     int rank;
     int offset;
     int contig_length;
     int stride[ESMF_MAXDIM];
     int rep_count[ESMF_MAXDIM];
     
// !PUBLIC MEMBER FUNCTIONS:
//

  public:
    int ESMC_XPacketSetDefault(int rank, int offset, int contig_length, 
                               int *stride, int *rep_count);

 // accessor methods for class members
    int ESMC_XPacketGet(int *nrank, int *noffset, int *ncontig_length, 
                        int *nstride, int *nrep_count);
    //int ESMC_XPacketSet(<value type>  value);

 // get/set methods for internal data
    int ESMC_XPacketSetEmpty(void);

    int ESMC_XPacketSetOffset(int offset) { this->offset = offset;
                                            return ESMF_SUCCESS; }

    int ESMC_XPacketGetOffset(void) { return this->offset; }

 // miscellaneous fun stuff
    int ESMC_XPacketIntersect(ESMC_XPacket *xpacket1, 
                              ESMC_XPacket *xpacket2);
    int ESMC_XPacketGlobalToLocal(ESMC_XPacket *global_XP,
                                  ESMC_AxisIndex *indexlist, int rank,
                                  int *global_start);
                         
    int ESMC_XPacketEmpty();      // returns true (1) if the xp is empty
    int ESMC_XPacketPrint(const char *options);

 // native C++ constructors/destructors
	ESMC_XPacket(void);
	~ESMC_XPacket(void);
  
  
    friend int ESMC_XPacketFromAxisIndex(struct ESMC_AxisIndex *indexlist,
                                         int size_axisindex, int *global_count,
                                         ESMC_Logical (*boundary)[2],
                                         ESMC_XPacket **xp_list, int *xp_count);
    friend int ESMC_XPacketMakeBuffer(int xpCount, ESMC_XPacket **xpList,
                                      int VMType, int nbytes, char **buffer,
                                      int *bufferSize);
    friend int ESMC_XPacketPackBuffer(int xpCount, ESMC_XPacket **xpList,
                                      int VMType, int nbytes, void *dataAddr,
                                      char *buffer);
    friend int ESMC_XPacketUnpackBuffer(int xpCount, ESMC_XPacket **xpList,
                                      int VMType, int nbytes, char *buffer,
                                      void *dataAddr);

    friend int ESMC_XPacketGetEmpty(int *nrank, int *noffset, 
                                    int *ncontig_length, 
                                    int *nstride, int *nrep_count);

// !PRIVATE MEMBER FUNCTIONS:
//
  private: 
//
//
//EOP
//-----------------------------------------------------------------------------

 };   // end class ESMC_XPacket

 // non-class method - creates a list of xp's to return
    int ESMC_XPacketFromAxisIndex(struct ESMC_AxisIndex *indexlist,
                                  int size_axisindex, int *global_count,
                                  ESMC_Logical (*boundary)[2],
                                  ESMC_XPacket **xp_list, int *xp_count);

    int ESMC_XPacketMakeBuffer(int xpCount, ESMC_XPacket **xpList,
                               int VMType, int nbytes, char **buffer,
                               int *bufferSize);
    int ESMC_XPacketPackBuffer(int xpCount, ESMC_XPacket **xpList,
                               int VMType, int nbytes, void *dataAddr,
                               char *buffer);
    int ESMC_XPacketUnpackBuffer(int xpCount, ESMC_XPacket **xpList,
                               int VMType, int nbytes, char *buffer,
                               void *dataAddr);
    int ESMC_XPacketGetEmpty(int *nrank, int *noffset, int *ncontig_length, 
                             int *nstride, int *nrep_count);

 #endif  // ESMC_XPacket_H
