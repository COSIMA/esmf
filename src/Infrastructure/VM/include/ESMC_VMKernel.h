// $Id: ESMC_VMKernel.h,v 1.9.2.2 2005/03/02 05:32:18 theurich Exp $
//
// Earth System Modeling Framework
// Copyright 2002-2003, University Corporation for Atmospheric Research, 
// Massachusetts Institute of Technology, Geophysical Fluid Dynamics 
// Laboratory, University of Michigan, National Centers for Environmental 
// Prediction, Los Alamos National Laboratory, Argonne National Laboratory, 
// NASA Goddard Space Flight Center.
// Licensed under the GPL.

//-----------------------------------------------------------------------------
//
// !DESCRIPTION:
//
//
//-----------------------------------------------------------------------------

#ifndef ESMC_VMKERNEL_H
#define ESMC_VMKERNEL_H

#include <pthread.h>
#include <mpi.h>

// reduction operations
enum vmOp   { vmSUM=1, vmMIN, vmMAX};
enum vmType { vmI4=1, vmR4, vmR8};

// define the communication preferences
#define PREF_INTRA_PROCESS_SHMHACK    (0)       // default
#define PREF_INTRA_PROCESS_PTHREAD    (1)

#define PREF_INTRA_SSI_POSIXIPC       (0)
#define PREF_INTRA_SSI_MPI1           (1)       // default

#define PREF_INTER_SSI_MPI1           (0)       // default

// - buffer lenghts in bytes
#define PIPC_BUFFER                   (4096)
#define SHARED_BUFFER                 (64)

// begin sync stuff -----
#define SYNC_NBUFFERS                 (2)
typedef struct{
  // hack sync variables
  // The sole purpose of the last index on all of these variables is to provide
  // a tuning parameter! Changing the extend of this dimension will modify 
  // whether a variable takes the space of a whole cache line or not.
  volatile int a_done[1];
  volatile int b_done[1];
  volatile int buffer_done[SYNC_NBUFFERS][1];
}shmsync;  

void sync_a_flip(shmsync *shms);
void sync_a_flop(shmsync *shms);
void sync_b_flip(shmsync *shms);
void sync_b_flop(shmsync *shms);
void sync_buffer_flag_fill(shmsync *shms, int select);
void sync_buffer_flag_empty(shmsync *shms, int select);
void sync_buffer_wait_fill(shmsync *shms, int select);
void sync_buffer_wait_empty(shmsync *shms, int select);
void sync_reset(shmsync *shms);
// end sync stuff -----


typedef struct vmk_ch{
  struct vmk_ch *prev_handle;   // previous handle in the queue
  struct vmk_ch *next_handle;   // next handle in the queue
  int nelements;    // number of elements
  int type;         // 0: commhandle container, 1: MPI_Requests, 2: ... reserved
  struct vmk_ch **handles;   // sub handles
  MPI_Request *mpireq; // request array
}vmk_commhandle;


typedef struct{
  // hack sync variables
  shmsync shms;
  // name of SHM resource
  char shm_name[80];
  // buffer
  char buffer[2][PIPC_BUFFER];
}pipc_mp;


typedef struct{
  // source and destination pointers
  volatile void *ptr_src;
  volatile void *ptr_dest;
  // hack sync variables
  shmsync shms;
  // buffer for small messages
  char buffer[SHARED_BUFFER];
  // Pthread sync variables
  pthread_mutex_t mutex1;
  pthread_cond_t cond1;
  pthread_mutex_t mutex2;
  pthread_cond_t cond2;
  volatile int tcounter;
}shared_mp;


typedef struct{
  int comm_type;    // communication type
//gjt - don't use yet:  int mpitag_send;  // tag for MPI communications
//gjt - don't use yet:  int mpitag_recv;  // tag for MPI communications
  shared_mp *shmp;  // shared memory message passing structure
  pipc_mp *pipcmp;  // posix ipc message passing structure
}comminfo;

class ESMC_VMK{
  // members
  protected:
    int mypet;          // PET id of this instance
    pthread_t mypthid;  // my pthread id
    // pet -> core mapping
    int npets;      // number of PETs in this ESMC_VMK
    int *lpid;      // local pid (equal to rank in local MPI context)
    int *pid;       // pid (equal to rank in MPI_COMM_WORLD)
    int *tid;       // thread index
    int *ncpet;     // number of cores this pet references
    int **cid;      // core id of the cores this pet references
    // general information about this ESMC_VMK
    int mpionly;    // 0: there is multi-threading, 1: MPI-only
    // MPI communication handles for MPI processes associated with this ESMC_VMK
    MPI_Group mpi_g;
    MPI_Comm mpi_c;
    // Shared mutex and thread_finish variables. These are pointers that will be
    // pointing to shared memory variables between different thread-instances of
    // the ESMC_VMK object.
    pthread_mutex_t *pth_mutex2;
    pthread_mutex_t *pth_mutex;
    int *pth_finish_count;
    // Communications array
    comminfo **commarray;   // this array is shared between pets of same pid
    // Communication requests queue
    int nhandles;
    vmk_commhandle *firsthandle;
    // static info of physical machine
    static int ncores; // total number of cores in the physical machine
    static int *cpuid; // cpuid associated with certain core (multi-core cpus)
    static int *ssiid; // single system inmage id to which this core belongs
  public:
    // static MPI info, MPI_COMM_WORLD Group and Comm of the default ESMC_VMK
    static MPI_Group default_mpi_g;
    static MPI_Comm default_mpi_c;
    // static variables that hold command line arguments
    static int argc;
    static char **argv;

  // methods
  private:
    void vmk_obtain_args(void);
    void vmk_commhandle_add(vmk_commhandle *commhandle);
    int vmk_commhandle_del(vmk_commhandle *commhandle);
  public:
    void vmk_init(void);
      // initialize the physical machine and a default (all MPI) virtual machine
    void vmk_finalize(void);
      // finalize default (all MPI) virtual machine
    void vmk_abort(void);
      // abort default (all MPI) virtual machine

    void vmk_construct(int mypet, pthread_t pthid, int npets, int *lpid, 
      int *pid, int *tid, int *ncpet, int **cid, MPI_Group mpi_g, 
      MPI_Comm mpi_c, pthread_mutex_t *pth_mutex2, pthread_mutex_t *pth_mutex,
      int *pth_finish_count, comminfo **commarray, int pref_intra_ssi);
      // fill an already existing ESMC_VMK object with info
    void vmk_destruct(void);
      // free allocations within an existing ESMC_VMK object

    void *vmk_startup(class ESMC_VMKPlan *vmp, void *(fctp)(void *, void *),
      void *cargo, int *rc);
      // enter a vm derived from current vm according to the ESMC_VMKPlan
    void vmk_enter(class ESMC_VMKPlan *vmp, void *arg, void *argvmkt);
    void vmk_exit(class ESMC_VMKPlan *vmp, void *arg);
    void vmk_shutdown(class ESMC_VMKPlan *vmp, void *arg);
      // exit a vm derived from current vm according to the ESMC_VMKPlan
  
    void vmk_print(void);
    
    // info calls
    int vmk_npets(void);           // return npets
    int vmk_mypet(void);           // return mypet
    pthread_t vmk_mypthid(void);   // return mypthid
    int vmk_ncpet(int i);          // return ncpet
    int vmk_ssiid(int i);          // return ssiid
    MPI_Comm vmk_mpi_comm(void);   // return mpi_c
    int vmk_nthreads(int i);       // return number of threads in group PET
    int vmk_tid(int i);            // return tid for PET
    int vmk_pid(int i);            // return pid for PET
    
    
    // communication calls
    void vmk_send(void *message, int size, int dest);
    void vmk_send(void *message, int size, int dest,
      vmk_commhandle **commhandle);
    void vmk_recv(void *message, int size, int source);
    void vmk_recv(void *message, int size, int source,
      vmk_commhandle **commhandle);
    void vmk_barrier(void);
    
    // newly written communication calls
    void vmk_sendrecv(void *sendData, int sendSize, int dst,
      void *recvData, int recvSize, int src);
    void vmk_sendrecv(void *sendData, int sendSize, int dst,
      void *recvData, int recvSize, int src, vmk_commhandle **commhandle);
    void vmk_threadbarrier(void);
    void vmk_allreduce(void *in, void *out, int len, vmType type, vmOp op);
    void vmk_allglobalreduce(void *in, void *out, int len, vmType type,
      vmOp op);
    void vmk_scatter(void *in, void *out, int len, int root);
    void vmk_gather(void *in, void *out, int len, int root);
    void vmk_broadcast(void *data, int len, int root);

    void vmk_wait(vmk_commhandle **commhandle);
    void vmk_waitqueue(void);

  // friend classes
  friend class ESMC_VMKPlan;
};


class ESMC_VMKPlan{
  public:
    int npets;
    int nothreadflag; // 0-default threaded VM, 1-non-threaded VM
    int parentVMflag; // 0-create child VM, 1-run on parent VM
    int *spawnflag;   // for each pet: 0-don't spawn, >=1-spawn threads
    int *contribute;  // pet id to which non-spawning pet contributes its cores
    int *cspawnid;    // idication to which one of spawned pets to contribute to
    // ESMC_VMK references for this PET (as many entries as this PET spawns)
    int nspawn;       // number of PETs this PET will spwan
    ESMC_VMK **myvms; // this array holds pointers to heap ESMC_VMK instances
    // Communication preferences
    // These preferences will be satisfied if the architecture supports it, 
    // otherwise the default communication setting is chosen instead.
    int pref_intra_process;     // default: PREF_INTRA_PROCESS_SHMHACK
    int pref_intra_ssi;         // default: PREF_INTRA_SSI_POSIXIPC
    int pref_inter_ssi;         // defualt: PREF_INTER_SSI_MPI1
        
  public:
    ESMC_VMKPlan(void);
      // native constructor (sets communication preferences to defaults)
    ~ESMC_VMKPlan(void);
      // native destructor
    void vmkplan_garbage(void);
      // perform garbage collection within a ESMC_VMKPlan object
    int vmkplan_nspawn(void);
      // return number of PETs that are being spawned out of current PET
    void vmkplan_myvms(ESMC_VMK **myvms);
      // set the internal myvms pointer array
    void vmkplan_useparentvm(ESMC_VMK &vm);
      // use the parent VM, don't create new context
    void vmkplan_maxthreads(ESMC_VMK &vm);  
      // set up a ESMC_VMKPlan that will maximize the number of thread-pets
    void vmkplan_maxthreads(ESMC_VMK &vm, int max);  
      // set up a ESMC_VMKPlan that will max. number of thread-pets up to max
    void vmkplan_maxthreads(ESMC_VMK &vm, int max, 
      int pref_intra_process, int pref_intra_ssi, int pref_inter_ssi);
      // set up a ESMC_VMKPlan that will max. number of thread-pets up to max
    void vmkplan_maxthreads(ESMC_VMK &vm, int max, int *plist, int nplist);  
      // set up a ESMC_VMKPlan that will max. number of thread-pets up to max
      // but only allow PETs listed in plist to participate
    int vmkplan_maxthreads(ESMC_VMK &vm, int max, int *plist, int nplist,
      int pref_intra_process, int pref_intra_ssi, int pref_inter_ssi); 
      // set up a ESMC_VMKPlan that will max. number of thread-pets up to max
      // but only allow PETs listed in plist to participate
    void vmkplan_minthreads(ESMC_VMK &vm);
      // set up a ESMC_VMKPlan that will only have single threaded pet
      // instantiations and claim all cores of pets that don't make it through
    void vmkplan_minthreads(ESMC_VMK &vm, int max);
      // set up a ESMC_VMKPlan that will only have single threaded pet
      // instantiations and claim all cores of pets that don't make it through,
      // up to max cores per pet
    void vmkplan_minthreads(ESMC_VMK &vm, int max, int *plist, int nplist);
      // set up a ESMC_VMKPlan that will only have single threaded pet
      // instantiations and claim all cores of pets that don't make it through,
      // up to max cores per pet but only allow PETs listed in plist to
      // participate
    int vmkplan_minthreads(ESMC_VMK &vm, int max, int *plist, int nplist,
      int pref_intra_process, int pref_intra_ssi, int pref_inter_ssi); 
      // set up a ESMC_VMKPlan that will only have single threaded pet
      // instantiations and claim all cores of pets that don't make it through,
      // up to max cores per pet but only allow PETs listed in plist to
      // participate
    void vmkplan_maxcores(ESMC_VMK &vm);
      // set up a ESMC_VMKPlan that will have pets with the maximum number of
      // cores available
    void vmkplan_maxcores(ESMC_VMK &vm, int max);
      // set up a ESMC_VMKPlan that will have pets with the maximum number of
      // cores available, but not more than max
    void vmkplan_maxcores(ESMC_VMK &vm, int max, int *plist, int nplist);
      // set up a ESMC_VMKPlan that will have pets with the maximum number of
      // cores available, but not more than max and only use PETs listed in
      // plist
    int vmkplan_maxcores(ESMC_VMK &vm, int max, int *plist, int nplist,
      int pref_intra_process, int pref_intra_ssi, int pref_inter_ssi); 
      // set up a ESMC_VMKPlan that will have pets with the maximum number of
      // cores available, but not more than max and only use PETs listed in
      // plist
    void vmkplan_print(void);  

  friend class ESMC_VMK;
  
};

#endif  // ESMC_VMKERNEL_H
