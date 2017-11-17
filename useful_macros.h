/*
 * #include "useful_macros.h"
 *
 *  Created on: 17 θώνÿ 2014 γ.
 *      Author: 1
 */
#ifndef USEFUL_MACROS_H_
#define USEFUL_MACROS_H_
#include <string.h>

#define ARRAY_NELEM(ARR)  (sizeof(ARR)/(sizeof((ARR)[0])))

/************* Array based queue tail/head manipulation when tail and head are indexes ************/
#define IQUEUE_INCREMENT(TAIL, QARRAY, NELEM) do{\
++(TAIL);\
if((TAIL) == (ARRAY_NELEM(QARRAY)))\
  (TAIL) = 0;\
++(NELEM); \
}while(0)

#define IQUEUE_DECREMENT(HEAD, QARRAY, NELEM) do{\
++(HEAD);\
if((HEAD) == (ARRAY_NELEM(QARRAY)))\
  HEAD = 0;\
--(NELEM); \
}while(0)

#define IQUEUE_INIT(HEAD,TAIL,NELEM) do{\
 (HEAD) = (TAIL) = (NELEM) = 0; \
}while(0)

/************* Array based queue tail/head manipulation when tail and head are pointers ************/

#define PQUEUE_INCREMENT(TAIL, QARRAY, NELEM) do{\
++(TAIL);\
if((TAIL) == &((QARRAY)[ARRAY_NELEM(QARRAY)]))\
  (TAIL) = (QARRAY);\
++(NELEM); \
}while(0)

#define PQUEUE_DECREMENT(HEAD, QARRAY, NELEM) do{\
++(HEAD);\
if((HEAD) == &(QARRAY[ARRAY_NELEM(QARRAY)]))\
  HEAD = (QARRAY);\
--(NELEM); \
}while(0)

#define PQUEUE_INIT(HEAD,TAIL, QARRAY, NELEM) do{\
 (HEAD) = (TAIL) = (QARRAY);\
 (NELEM) = 0; \
}while(0)

/************************************** short / long - assembling from bytes *******************/

struct two_bytes {
    char lsb;
    char msb;
};

struct four_bytes {
    char b0;
    char b1;
    char b2;
    char b3;
};

struct eight_bytes {
    char b0;
    char b1;
    char b2;
    char b3;
    char b4;
    char b5;
    char b6;
    char b7;
};


#define _LSB(X)   ((uint8_t)(X))
#define _MSB(X)   ((uint8_t)((X)>>8))

#define TWOCHAR2SHORT(H,L)             ((short)(((unsigned char)(H)<<8)|((unsigned char)(L))))
#define FOURCHAR2LONG(B3,B2,B1,B0)     ((long) (((unsigned char)(B3)<<24)|((unsigned char)(B2)<<16)|((unsigned char)(B1)<<8)|((unsigned char)(B0))))

#define EIGHTCHAR2LONGLONG(B7,B6,B5,B4,B3,B2,B1,B0) \
    ((long long) \
           (                           \
            ( (unsigned long long) ((unsigned char)(B7))   <<56) |\
            ( (unsigned long long) ((unsigned char)(B6))   <<48) |\
            ( (unsigned long long) ((unsigned char)(B5))   <<40) |\
            ( (unsigned long long) ((unsigned char)(B4))   <<32) |\
            ( (unsigned long long) ((unsigned char)(B3))   <<24) |\
            ( (unsigned long long) ((unsigned char)(B2))   <<16) |\
            ( (unsigned long long) ((unsigned char)(B1))   <<8)  |\
              (unsigned long long) ((unsigned char)(B0))       \
           )                           \
    )

#define BYTES2SHORT(_STRU_)            TWOCHAR2SHORT(_STRU_.msb , _STRU_.lsb)
#define BYTES2INT(_STRU_)              FOURCHAR2LONG(_STRU_.b3,_STRU_.b2,_STRU_.b1,_STRU_.b0)
#define BYTES2LONGLONG(_STRU_)  EIGHTCHAR2LONGLONG(_STRU_.b7,_STRU_.b6,_STRU_.b5,_STRU_.b4, \
                                                   _STRU_.b3,_STRU_.b2,_STRU_.b1,_STRU_.b0)

/************************************************************************************************/

#define SHORT2BYTES(_STRU_, _SHORT_) do{ (_STRU_).lsb = (char)(_SHORT_); (_STRU_).msb = (char)((_SHORT_)>>8); }while(0)

#define INT2BYTES(_STRU_, _INT_) \
do{ (_STRU_).b0 = (char)(_INT_); \
    (_STRU_).b1 = (char)((_INT_)>>8);\
    (_STRU_).b2 = (char)((_INT_)>>16);\
    (_STRU_).b3 = (char)((_INT_)>>24);\
}while(0)

#define LONGLONG2BYTES(_STRU_, _INT_) \
do{ (_STRU_).b0 = (char)(_INT_); \
    (_STRU_).b1 = (char)((_INT_)>>8);\
    (_STRU_).b2 = (char)((_INT_)>>16);\
    (_STRU_).b3 = (char)((_INT_)>>24);\
    (_STRU_).b4 = (char)((_INT_)>>32);\
    (_STRU_).b5 = (char)((_INT_)>>40);\
    (_STRU_).b6 = (char)((_INT_)>>48);\
    (_STRU_).b7 = (char)((_INT_)>>56);\
}while(0)

//HOST TO CONTROLLER VIA COMPORT ALWAYS LITTLE ENDIAN
#include <stdint.h>
union ieee754_float
{
    float f;
    uint32_t d32;
    /* This is the IEEE 754 single-precision format.  */
    struct
    {
        uint32_t negative :1;
        uint32_t exponent :8;
        uint32_t mantissa :23;
    } big;
    struct
    {
        uint32_t mantissa :23;
        uint32_t exponent :8;
        uint32_t negative :1;
    } little;

    struct four_bytes data;
};

/*#ifdef BIG_ENDIAN
 #define FLOAT2BYTES(_STRU_,_FLOAT_) \
do{\
  ((union ieee754_float)(_STRU_)).little.mantissa =  ((union ieee754_float)(_FLOAT_)).big.mantissa; \
  ((union ieee754_float)(_STRU_)).little.exponent =  ((union ieee754_float)(_FLOAT_)).big.exponent; \
  ((union ieee754_float)(_STRU_)).little.negative =  ((union ieee754_float)(_FLOAT_)).big.negative; \
}while(0)

 static __inline float BYTES2FLOAT(struct four_bytes data)
 {
 union ieee754_float tmp;
 ((union ieee754_float)(tmp)).big.mantissa =  ((union ieee754_float)(data)).little.mantissa;
 ((union ieee754_float)(tmp)).big.exponent =  ((union ieee754_float)(data)).little.exponent;
 ((union ieee754_float)(tmp)).big.negative =  ((union ieee754_float)(data)).little.negative;

 return tmp.f
 }

 #else
 */

#define FLOAT2BYTES(_STRU_,_FLOAT_)   ((union ieee754_float*)&(_STRU_))->d32=((const union ieee754_float *)(&(_FLOAT_)))->d32
//#define BYTES2FLOAT(data) ((union ieee754_float *)(&data))->f
static __inline float BYTES2FLOAT(struct four_bytes addr) {
	float temp;
	memcpy(&temp, &addr, 4);
	return temp;
}


//#endif

/************************************************/

#define HIGH_BYTE(X)    (uint8_t)((X)>>8)
#define LOW_BYTE(X)     (uint8_t)(X)

/************************************************/
// Useful macros to make linear "switch based" state-machines

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

