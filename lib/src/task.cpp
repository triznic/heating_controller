/*-------------------------------------------------------------------

   Task module 
 
 -------------------------------------------------------------------*/

 #define __TASK_INTERNAL_H__
 #include "task.h"

// --- Externals ----------------------------------------------------

// --- Static variables ---------------------------------------------
static Task_T tasks[MAX_TASKS];
static int    tasks_cnt = 0;

// --- Functions ----------------------------------------------------
void Task_Init(void)
{
    memset(tasks, sizeof(tasks), 0);  
}

bool Task_Add(void (*fnc)(void), int period, bool enable)
{
    bool result = false;

    if (tasks_cnt < MAX_TASKS) 
    {
        tasks[tasks_cnt].fnc = fnc;
        tasks[tasks_cnt].period = period;
        tasks[tasks_cnt].cnt = period;
        tasks[tasks_cnt].enable = enable;
        tasks_cnt++;
        result = true;
    }
    return result;
}

bool Task_SetPeriod(void (*fnc)(void), int period)
{
    int loop;
    bool result = false;

    for (loop = 0; loop < tasks_cnt; loop++)
    {
        if (fnc == tasks[loop].fnc)
        {
            tasks[loop].period = period;
            tasks[loop].cnt = period;
            result = true; 
            break;
        }  
    }
    return result;
}
bool Task_SetEnable(void (*fnc)(void), bool enable)
{
    int loop;
    bool result = false;

    for (loop = 0; loop < tasks_cnt; loop++)
    {
        if (fnc == tasks[loop].fnc)
        {
           tasks[loop].enable = enable;
           result = true; 
           break;
        }
    }
    return result;
}
void Task_Process(void)
{
    static uint32_t time = 0;
    uint32_t currTime;

    currTime = millis();
    if (currTime != time)
    {
        Task_Inc();
        time = currTime;  
    }
    Task_Run();
}

// --- Local Functions ----------------------------------------------
static void Task_Inc(void)
{
    int loop;

    for (loop = 0; loop < tasks_cnt; loop++)
    {
        if  (tasks[loop].enable == true)
        {
          // decrement
          if  (tasks[loop].cnt > 0)
            tasks[loop].cnt--;
        }
    }
}

static void Task_Run(void)
{
    int loop;

    for (loop = 0; loop < tasks_cnt; loop++)
    {
        if (tasks[loop].enable == true)
        {
            if (tasks[loop].cnt == 0)
            {
              tasks[loop].fnc();
              tasks[loop].cnt = tasks[loop].period;
            }
        }   
    }
}

