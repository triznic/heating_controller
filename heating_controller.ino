/*
 *  v 1.1 - 10/15/2018
 *    - task.h/.cpp added
 * 
 * 
 * 
 * 
 */
#include "hmi.h"
#include "task.h"
#include "temp_meas.h"

const int VoutAnalog = A0;
const int VinAnalog = A1;
float R2, R1, Vin, Vout;
float Temp, R0, R100;



// HeartBeat Task
void Led_HeartBeat(void)
{
  static int led = 0;

  led ^= 0x01;
  digitalWrite(LED_BUILTIN, led);
}


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Task_Init();
  Task_Add(Led_HeartBeat, 500,  true);

  Temp_Meas_Init();
  Temp_ChangeState(TEMP_STOVE_OUT,SENS_CONNECTED);
  Temp_ChangeType(TEMP_STOVE_OUT,SENSOR_15KOHM);
  Task_Add(Temp_Process, 1000,  true);


  
  //R0 = 190;
  //R100 = 893;
  //R1 = 14970; // Ohm
  // put your setup code here, to run once:
  Serial.begin(9600);
  

  hmi_init();
  Task_Add(hmi_process, 100,  true);
}

void loop() {
  Task_Process();

  //hmi_temp(tempSensors[TEMP_STOVE_OUT].tempValue);
}
