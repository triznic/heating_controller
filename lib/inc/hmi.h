/*-------------------------------------------------------------------
 *  Temperature Measurement module
 *------------------------------------------------------------------*/

#ifndef __HMI_H__
#define __HMI_H__

#include "Arduino.h"

// --- Global Constants ----------------------------------------------


// --- Type Definition ----------------------------------------------
typedef enum
{
  HMI_TEMP_PRIVOD,
  HMI_TEMP_SPIATOCKA,
  HMI_TEMP_PEC,
  HMI_TEMP_NADRZ_TOP,
  HMI_TEMP_NADRZ_BOT,
  HMI_TEMP_SMALL_LOOP,
  HMI_TEMP_REQ_SMALL_LOOP,
  HMI_TEMP_BIG_LOOP,
  HMI_TEMP_REQ_BIG_LOOP,
  /*HMI_TEMP_REQ_TANK,
  TEMP_STOVE_IN,
  TEMP_STOVE,
  TEMP_TANK_TOP,
  TEMP_TANK_BOTTON,
  TEMP_BOILER,*/
  HMI_CNT
}HMIstates_E;

typedef struct
{
  HMIstates_E hmiID;
  void (*fun_ptr)(void);
}HMIstate_T;

//extern HMIstate_T hmiStates[HMI_CNT];

// --- Functions ----------------------------------------------------
void hmi_init(void);
void hmi_process(void);
float hmi_readReqSmallLoop(void);
float hmi_readReqBigLoop(void);

#endif /* __HMI_H__ */
