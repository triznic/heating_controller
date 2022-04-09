#include "temp_meas.h"

// --- Local Constants -----------------------------------------------
#define TEMP_AN_STOVE_OUT    A0
#define TEMP_AN_STOVE_IN     A1
#define TEMP_AN_STOVE        A2
#define TEMP_AN_TANK_TOP     A3
#define TEMP_AN_TANK_BOTTON  A4
#define TEMP_AN_BOILER       A5

#define SEN_15KOHM_KOEF_A0  0.0001707F
#define SEN_15KOHM_KOEF_A1  -0.0425697F
#define SEN_15KOHM_KOEF_A2  1.9276630F

#define SEN_PT100_KOEF_A0  0.0001707F
#define SEN_PT100_KOEF_A1  -0.0425697F
#define SEN_PT100_KOEF_A2  1.9276630F

// --- Local Macros -----------------------------------------------

// --- External ------------------------------------------------------
Temp_T tempSensors[TEMP_CNT]
{
  { .tempID = (TempSensors_E)0,
    .tempAnPin = A0 + 0,
    .tempType = SENSOR_15KOHM,
    .tempValue = 25,
    .tempState = SENS_CONNECTED
  },
  { .tempID = (TempSensors_E)1,
    .tempAnPin = A0 + 1,
    .tempType = SENSOR_15KOHM,
    .tempValue = 25,
    .tempState = SENS_UNCONNECTED
  },
  { .tempID = (TempSensors_E)2,
    .tempAnPin = A0 + 2,
    .tempType = SENSOR_15KOHM,
    .tempValue = 25,
    .tempState = SENS_UNCONNECTED
  },
  { .tempID = (TempSensors_E)3,
    .tempAnPin = A0 + 3,
    .tempType = SENSOR_15KOHM,
    .tempValue = 25,
    .tempState = SENS_UNCONNECTED
  },
  { .tempID = (TempSensors_E)4,
    .tempAnPin = A0 + 4,
    .tempType = SENSOR_15KOHM,
    .tempValue = 25,
    .tempState = SENS_UNCONNECTED
  }
};

// --- Static variables ---------------------------------------------


// --- Functions ----------------------------------------------------
void Temp_Meas_Init(void)
{
  int i;
  int analogPinA0;
  analogPinA0 = A0;
  
  for (i=0;i<TEMP_CNT;i++)
  {
    tempSensors[i].tempID = (TempSensors_E)i;
    tempSensors[i].tempAnPin = analogPinA0 + i;
    tempSensors[i].tempType = SENSOR_15KOHM;
    tempSensors[i].tempValue = 0;
    tempSensors[i].tempState = SENS_UNCONNECTED;
  }
  // Just first temp is connected
  tempSensors[TEMP_STOVE_OUT].tempState = SENS_CONNECTED;
  //tempSensors[TEMP_STOVE].tempState = SENS_CONNECTED;  
}

TempState_E Temp_isConnected(TempSensors_E num)
{
  return tempSensors[num].tempState;
}

float Temp_ReadVal(TempSensors_E num)
{
  return tempSensors[num].tempValue;
}

void Temp_ChangeType(TempSensors_E sensor, TempType_E tempType)
{
  tempSensors[sensor].tempType = tempType;
}

void Temp_ChangeState(TempSensors_E sensor, TempState_E tempState)
{
  tempSensors[sensor].tempState = tempState;
}

void Temp_Process(void)
{
  int i;
  float rawData;
  
  for (i=0;i<TEMP_CNT;i++)
  {
    if (tempSensors[i].tempState == SENS_CONNECTED)
    {
      // Read analog raw data
      rawData = analogRead(tempSensors[i].tempAnPin);

      // Calculated temp according to the sensor's type
      if (tempSensors[i].tempType == SENSOR_PT100)
      {
        tempSensors[i].tempValue = SEN_PT100_KOEF_A0 * rawData * rawData + SEN_PT100_KOEF_A1 * rawData + SEN_PT100_KOEF_A2;
      }
      else if (tempSensors[i].tempType == SENSOR_15KOHM)
      {
        tempSensors[i].tempValue = SEN_15KOHM_KOEF_A0 * rawData * rawData + SEN_15KOHM_KOEF_A1 * rawData + SEN_15KOHM_KOEF_A2;
      }
    }
  }
}

