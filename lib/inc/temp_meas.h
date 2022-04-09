/*-------------------------------------------------------------------
 *  Temperature Measurement module
 *------------------------------------------------------------------*/

#ifndef __TEMP_MEAS_H__
#define __TEMP_MEAS_H__

#include <Arduino.h>

// --- Global Constants ----------------------------------------------


// --- Type Definition ----------------------------------------------
typedef enum
{
  TEMP_STOVE_OUT,
  TEMP_STOVE_IN,
  TEMP_STOVE,
  TEMP_TANK_TOP,
  TEMP_TANK_BOTTON,
  //TEMP_BOILER,
  TEMP_CNT
}TempSensors_E;

typedef enum
{
  SENS_CONNECTED,
  SENS_UNCONNECTED
}TempState_E;

typedef enum
{
  SENSOR_PT100,   
  SENSOR_15KOHM
}TempType_E; 

typedef struct
{
  TempSensors_E tempID;
  int           tempAnPin;
  TempType_E    tempType;
  float         tempValue; 
  TempState_E   tempState;
}Temp_T;

extern Temp_T tempSensors[TEMP_CNT];

// --- Functions ----------------------------------------------------
void Temp_Meas_Init(void);
void Temp_Process(void);
void Temp_ChangeState(TempSensors_E sensor, TempState_E tempState);
void Temp_ChangeType(TempSensors_E sensor, TempType_E tempType);
float Temp_ReadVal(TempSensors_E num);
TempState_E Temp_isConnected(TempSensors_E num);

#ifdef __TEMP_MEAS_INTERNAL_H__

// --- Local Functions ---------------------------------------------



#endif /* __TEMP_MEAS_INTERNAL_H__ */

#endif /* __TEMP_MEAS_H__ */
