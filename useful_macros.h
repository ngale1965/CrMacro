#ifndef USEFUL_MACROS_H_
#define USEFUL_MACROS_H_
#include <string.h>

// Useful macros to make linear "switch based" state-machines.
// States of the state-machine is stored in the integer variable.
// The initial state of the state machine is 0.
// Subsequent state values of the state machine are evaluated in the compilation time 
// and equal to __LINE__  

/** this macro opens coroutine part of code */
#define crBegin(STATE)   switch(STATE) { case 0:

/** wait till result not MYEWOULDBLOCK */
#define crWait0(RESULT) \
do{ \
if(RESULT==MYEWOULDBLOCK) { return;} \
}while(0)

#define crTimedWaitNACKAndUnlockIfError(STATE, RESULT, TIMER_PTR, TIMEOUT, LOCK) \
    do{ \
      if((RESULT==MYEWOULDBLOCK) || (RESULT==MYNACK) ) { \
       if(is_mytimer_expiry((TIMER_PTR), (TIMEOUT))) \
        {STATE = 0; LOCK = 0; RESULT = MYEINVAL; return RESULT;} \
       else { RESULT = MYEWOULDBLOCK; return RESULT;}\
      } \
      if(RESULT<0) { STATE = 0; LOCK = 0; return RESULT;} \
    }while(0)

#define crWaitNACK(STATE, RESULT) \
do{ \
if((RESULT==MYEWOULDBLOCK) || (RESULT==MYNACK) ) { RESULT = MYEWOULDBLOCK; return RESULT;}\
if(RESULT<0)              { STATE = 0; return RESULT;} \
}while(0)

#define crTimedWaitNACK(STATE, RESULT, TIMER_PTR, TIMEOUT) \
do{ \
  if((RESULT==MYEWOULDBLOCK) || (RESULT==MYNACK) ) { \
   if(is_mytimer_expiry((TIMER_PTR), (TIMEOUT))) \
    {STATE = 0; RESULT = MYEINVAL; return RESULT;} \
   else { RESULT = MYEWOULDBLOCK; return RESULT;}\
  } \
  if(RESULT<0) { STATE = 0; return RESULT;} \
}while(0)

/*****************************************************/

#define crTimedWait(STATE, RESULT, TIMER_PTR, TIMEOUT) \
do{ \
  if(RESULT==MYEWOULDBLOCK) { \
  if(is_mytimer_expiry((TIMER_PTR), (TIMEOUT))) \
    {STATE = 0; RESULT = MYEINVAL; return RESULT;} \
   else return RESULT; \
  } \
  if(RESULT<0) { STATE = 0; return RESULT;} \
}while(0)

#define crTimedMark(STATE, TIMER_PTR) \
 do{ set_mytimer(TIMER_PTR); STATE = __LINE__; case __LINE__:;}while(0)

#define crWait(STATE, RESULT) \
do{ \
if(RESULT==MYEWOULDBLOCK) { return RESULT;} \
if(RESULT<0)              { STATE = 0; return RESULT;} \
}while(0)
/////////////////////////////////////////////
#define crTimedWaitAndUnlockIfError(STATE, RESULT, TIMER_PTR, TIMEOUT, LOCK) \
do{ \
  if(RESULT==MYEWOULDBLOCK){ \
    if(is_mytimer_expiry((TIMER_PTR), (TIMEOUT))) \
      {STATE = 0; RESULT = MYEINVAL; LOCK = 0; return RESULT;} \
    else return RESULT; \
  }\
  if(RESULT<0) { STATE = 0; LOCK = 0;  return RESULT;} \
}while(0)

#define crWaitAndUnlockIfError(STATE, RESULT, LOCK) \
do{ \
if(RESULT==MYEWOULDBLOCK) { return RESULT;} \
if(RESULT<0)              { STATE = 0; LOCK = 0;  return RESULT;} \
}while(0)


#define crMark(STATE) \
 do{ STATE = __LINE__; case __LINE__:;}while(0)

#define crYield(STATE) \
  do{ STATE = __LINE__; return; case __LINE__:;}while(0)

#define crYieldVal(STATE, VALUE) \
  do{ STATE = __LINE__; return VALUE; case __LINE__:;}while(0)

#define crNextState(STATE, RESULT) \
do{ \
if(RESULT==MYEINVAL)      { STATE = 0; return RESULT;} \
if(RESULT==MYEWOULDBLOCK) { return RESULT;} \
if(RESULT==MYEOK)         { STATE = __LINE__; case __LINE__:;} \
} while (0)

#define crLastState(STATE)  do{ STATE = 0;}while(0)

#define crLock(LOCK)  {if((LOCK)) return MYEWOULDBLOCK; else (LOCK)=1;}
#define crUnLock(LOCK) (LOCK)=0;

#define crFinish }

#define crUsleep(TIMER,STATE,TIMEOUT) \
do{ \
set_usec_timer(TIMER);\
STATE = __LINE__; case __LINE__:;\
if(MYEWOULDBLOCK==usec_timer_expiry((TIMER), (TIMEOUT))) { return MYEWOULDBLOCK;} \
}while(0)

#define crSleep(TIMER,STATE,TIMEOUT) \
do{ \
set_mytimer(TIMER);\
STATE = __LINE__; case __LINE__:;\
if(MYEWOULDBLOCK==is_mytimer_expiry((TIMER), (TIMEOUT))) { return MYEWOULDBLOCK;} \
}while(0)

#endif /* USEFUL_MACROS_H_ */

