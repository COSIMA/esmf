// $Id: ESMC_Alarm.h,v 1.3 2002/10/07 18:56:36 eschwab Exp $
#ifndef ESMC_ALARM_H
#define ESMC_ALARM_H

#include <ESMC_TimeInterval.h>
#include <ESMC_TimeInstant.h>
#include <pthread.h>

class ESMC_Alarm
{
//-------------------------------------------------------------------------
//BOP
//
// !CLASS: ESMC_Alarm
//
// !SUPERCLASSES:
//
// !AGGREGATE CLASSES:
//	ESMC_TimeInterval (1)
//	ESMC_TimeInstant  (4)
//
// !ASSOCIATE CLASSES:
//
// !FRIEND CLASSES:
//
// !PUBLIC DATA MEMBERS:
//
// !PUBLIC MEMBER FUNCTIONS:
  public:

    ESMC_Alarm(void);
    ~ESMC_Alarm(void);

	// (TMG 4.1, 4.7)
    int Init(ESMC_TimeInterval *RingInterval,
             ESMC_TimeInstant  *RingTime,
             ESMC_TimeInstant  *StopTime,
             bool Enabled);

    int Enable(void);    // TMG4.5.3
    int Disable(void);

    int TurnOn(void);    // TMG4.6: manually turn on/off
    int TurnOff(void);

    bool IsRinging(void);    // TMG 4.4: synchronous query for apps
    int CheckRingTime(bool positive);
                         // associated clock should invoke after advance:
                         // TMG4.4, 4.6
                         // Check for crossing RingTime in either positive or
                         //   negative direction
                         // Can be basis for asynchronous alarm reporting

    bool operator==(const ESMC_Alarm &) const; 

    int GetRingInterval(ESMC_TimeInterval *RingInterval); // TMG4.7
    int SetRingInterval(ESMC_TimeInterval *RingInterval); // TMG4.5.2, 4.7

    int GetRingTime(ESMC_TimeInstant *RingTime);    // TMG4.7, 4.8
    int SetRingTime(ESMC_TimeInstant *RingTime);    // TMG4.5.1, 4.7, 4.8

    int GetPrevRingTime(ESMC_TimeInstant *PrevRingTime);    // TMG 4.7, 4.8
    int SetPrevRingTime(ESMC_TimeInstant *PrevRingTime);    // TMG 4.7, 4.8

    int GetStopTime(ESMC_TimeInstant *StopTime);    // TMG 4.5.2, 4.7
    int SetStopTime(ESMC_TimeInstant *StopTime);    // TMG 4.5.2, 4.7

// !DESCRIPTION:
//
//    TMG 4.1, 4.2:  Multiple alarms may be instantiated and associated
//                   with a clock via clock methods
//
// !BUGS:
//
// !SEE ALSO:
//
// !REVISION HISTORY:
//
//  10Jun02   Earl Schwab  Initial code.
//
//EOP
//-------------------------------------------------------------------------

  private:
    ESMC_TimeInterval RingInterval;	// (TMG 4.5.2)
    ESMC_TimeInstant  RingTime;    // (TMG 4.5.1) (StartTime ??)
    ESMC_TimeInstant  PrevRingTime;
    ESMC_TimeInstant  StopTime;

    bool              Ringing;	  // (TMG 4.4)
    bool              Enabled;    // able to ring (TMG 4.5.3)
	
	int               ID;		  // used to distinguish among
                                  //   multiple clock alarms

    pthread_mutex_t   AlarmMutex;	// (TMG 7.5)
};

#endif // ESMC_ALARM_H
