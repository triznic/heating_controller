/*-------------------------------------------------------------------

   Task module 
 
 -------------------------------------------------------------------*/

#ifndef __TASK_H__
#define __TASK_H__

#include <Arduino.h>

// --- Global Constants ---------------------------------------------

// --- Global Types -------------------------------------------------

// --- External -----------------------------------------------------

// --- Functions ---------------------------------------------------- 
void Task_Init(void);
bool Task_Add(void (*fnc)(void), int period, bool enable);
bool Task_SetPeriod(void (*fnc)(void), int period);
bool Task_SetEnable(void (*fnc)(void), bool enable);
void Task_Process(void);

#ifdef __TASK_INTERNAL_H__

// --- Local Constants ----------------------------------------------
#define MAX_TASKS         48U

// --- Local Types --------------------------------------------------
typedef struct
{
  void (*fnc)(void);
  int period;
  int cnt;
  bool enable;  
} Task_T;

// --- Local Functions ---------------------------------------------
static void Task_Inc(void);
static void Task_Run(void);

#endif  /* __TASK_INTERNAL_H__ */

#endif  /* __TASK_H__ */
