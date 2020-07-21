# FreeRTOS_modification

Template application with FreeRTOS redudancy modifications. This project adds replicated tasks and timed tasks to FreeRTOS.

Project is writen using "System workbench for STM32" based on Eclipse. Tested on STM32F407G-DISC1 development board.

## How to use

Use this project as a template for a new project, or just copy the third_party/FreeRTOS folder and user_app_template/Inc/FreeRTOSConfig.h to the project. To use added FreeRTOS functionality simply include FreeRTOS.h and taks.h in your source file.

## Timed tasks (watchdog)

Timed tasks have an ability to track their own execution time. On initialization, time limit is set. If time limit is overreached error callback is called. Timed tasks make use of FreeRTOS timers.

### List of relevant added functions

* [xTaskCreateTimed](#cmd_xTaskCreateTimed) : Creates a timed task.
* [vTaskTimedReset](#cmd_vTaskTimedReset) : Resets the timer of timed task.

### Functions
<a name="cmd_xTaskCreateTimed"></a>
```C
BaseType_t xTaskCreateTimed( TaskFunction_t pxTaskCode,
                             const char * const pcName,
                             const configSTACK_DEPTH_TYPE usStackDepth,
                             void * const pvParameters,
                             UBaseType_t uxPriority,
                             TaskHandle_t * const pxCreatedTask,
                             TickType_t xWorstRunTime,
                             WorstTimeTimerCb_t pxTimerCallback )
```                             
Create a new timed task and add it to the list of tasks that are ready to
run. Timed task will call pxTimerCallback if its runtime is greater than
specified by xWorstRunTime.

Internally, within the FreeRTOS implementation, tasks use two blocks of
memory.  The first block is used to hold the task's data structures.  The
second block is used by the task as its stack.  If a task is created using
xTaskCreateTimed() then both blocks of memory are automatically dynamically
allocated inside the xTaskCreate() function.  (see
http://www.freertos.org/a00111.html). Static version of the function is not
implemented.

Input parameters:

- pvTaskCode - Pointer to the task entry function.  Tasks
must be implemented to never return (i.e. continuous loop).

- pcName - A descriptive name for the task.  This is mainly used to
facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
is 16.

- usStackDepth - The size of the task stack specified as the number of
variables the stack can hold - not the number of bytes.  For example, if
the stack is 16 bits wide and usStackDepth is defined as 100, 200 bytes
will be allocated for stack storage.

- pvParameters - Pointer that will be used as the parameter for the task
being created.

- uxPriority - The priority at which the task should run.  Systems that
include MPU support can optionally create tasks in a privileged (system)
mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
example, to create a privileged task at priority 2 the uxPriority parameter
should be set to ( 2 | portPRIVILEGE_BIT ).

- pvCreatedTask - Used to pass back a handle by which the created task
can be referenced.

- xWorstRunTime - Worst run time in ticks. Function creates a timer with
timeout specified with xWorstRunTime

- pxTimerCallback - Pointer to the function that will be called if the
timer with xWorstRunTime timeout times out.

Returns pdPASS if the task was successfully created and added to a ready
list, otherwise an error code defined in the file projdefs.h

Example usage:
``` C
// Task to be created.
void vTaskTimedCode( void * pvParameters )
{
   for( ;; )
   {
       // Task code goes here.

       // Reset the timer.
       vTaskTimedReset(NULL);
   }
}

// Function to be called if timer overflows.
void vTaskTimeoutCallback ( WorstTimeTimerHandle_t xTimer )
{
  configASSERT(strncmp( pcTimerGetName( xTimer ), "WorstTimeTimer",
              strlen( "WorstTimeTimer" ) ) != 0);

#   ifndef NDEBUG
  void * phOwnerTask = pvTimerGetTimerID( xTimer );
  printf("Timer %s overflowed\n", pcTimerGetName( xTimer ));
  printf("Task %s overflowed\n", pcTaskGetName( phOwnerTask ));
#   endif

  // Timeout callback code.

  // Maybe task deletion is needed. Calling vTaskDelete automatically deletes
  // the timer too. Do NOT delete the timer directly. That will cause
  // undefined behavior when deleting the task.
  vTaskDelete( pvTimerGetTimerID( xTimer ) );
}

// Function that creates a task.
void vOtherFunction( void )
{
static uint8_t ucParameterToPass;
TaskHandle_t xHandle = NULL;

   // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
   // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
   // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
   // the new task attempts to access it.
   xTaskCreateTimed( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle, pdMS_TO_TICKS(1 * 1000), vTaskTimeoutCallback );
   configASSERT( xHandle );

   // Use the handle to delete the task.
   if( xHandle != NULL )
   {
       vTaskDelete( xHandle );
   }
}
```
---
<a name="cmd_vTaskTimedReset"></a>
```C
void vTaskTimedReset( TaskHandle_t pxTaskHandle )
```
Reset the timer of the timed task.

- Warning - Shall only be used for timed tasks.

Input parameters:

- pxTaskHandle - Handle of the task whose timer shall be reset.
Passing a NULL handle results in reseting the timer of the calling task.

Example usage:
``` C
void vTimedTask( void * pvParameters )
{
    for( ;; )
    {
        // Task code goes here.

        vTaskTimedReset(NULL);
    }
}
```


## Replicated tasks (lockstep)

Replicated tasks have an ability to detect errors using at least two tasks performing identical operations. Tasks are independently processed by the processor. Output variables from tasks are compared in real time. In case of discrepancy in the output variables, an error callback is called where user can process the error.


### List of relevant added functions

* [xTaskCreateReplicated](#cmd_xTaskCreateReplicated) : Creates a timed task.
* [xTaskSetCompareValue](#cmd_xTaskSetCompareValue) : Sets a compare value for the calling task.
* [vTaskSyncAndCompare](#cmd_vTaskSyncAndCompare) : Syncronizes the replicated tasks and compares compare values.

### Functions

<a name="cmd_xTaskCreateReplicated"></a>
``` C
BaseType_t xTaskCreateReplicated( TaskFunction_t pxTaskCode,
                                  const char * const pcName,
                                  const configSTACK_DEPTH_TYPE usStackDepth,
                                  void * const pvParameters,
                                  UBaseType_t uxPriority,
                                  TaskHandle_t * const pxCreatedTask,
                                  uint8_t ucReplicatedType,
                                  RedundantValueErrorCb_t pxRedundantValueErrorCb )
```
Create a new replicated task and add it to the list of tasks that are ready to
run. Replicated task is used to achieve redundancy of the software at the
expense of slower execution. Task executes slower because it is replicated
two or three times. Depending  on the type chosen. On every call to
vTaskSyncAndCompare task is suspended until every replicated task arrives to
the same point. When every task is in the synchronization function comparison
is done. If any of the comparison results differ callback function
pxRedundantValueErrorCb is called. In the callback function user can access
the compare values and choose whether to delete all the tasks.

Internally, within the FreeRTOS implementation, tasks use two blocks of
memory.  The first block is used to hold the task's data structures.  The
second block is used by the task as its stack.  If a task is created using
xTaskCreateReplicated() then both blocks of memory are automatically
dynamically allocated inside the xTaskCreateReplicated() function.  (see
http://www.freertos.org/a00111.html). Static version of this function is not
implemented.

Input parameters:
- pvTaskCode - Pointer to the task entry function.  Tasks
must be implemented to never return (i.e. continuous loop).

- pcName - A descriptive name for the task.  This is mainly used to
 facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
 is 16.

- usStackDepth - The size of the task stack specified as the number of
variables the stack can hold - not the number of bytes.  For example, if
the stack is 16 bits wide and usStackDepth is defined as 100, 200 bytes
will be allocated for stack storage.

- pvParameters - Pointer that will be used as the parameter for the task
being created.

- uxPriority - The priority at which the task should run.  Systems that
include MPU support can optionally create tasks in a privileged (system)
mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
example, to create a privileged task at priority 2 the uxPriority parameter
should be set to ( 2 | portPRIVILEGE_BIT ).

- pvCreatedTask - Used to pass back a handle by which the created task
can be referenced.

- ucReplicatedType - Valid values: taskREPLICATED_NO_RECOVERY and
taskREPLICATED_RECOVERY. No recovery is faster as it created only two
instances, but recovery is not possible. Recovery creates three identical
tasks. Recovery is possible with 2 out of 3 logic.

- pxRedundantValueErrorCb - Function to be called when compare values do
not match. Return value determines whether calling redundant task will be
deleted.

Returns pdPASS if the task was successfully created and added to a ready
list, otherwise an error code defined in the file projdefs.h


Example usage:
``` C
// Task to be created.
void vTaskCode( void * pvParameters )
{
    for( ;; )
    {
        // Task code goes here.

        vTaskSyncAndCompare(&xCompareValue);
    }
}

// NOTE: This function is called from the redundant task and not daemon.
uint8_t ucCompareErrorCb (CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    // Iterate through compare values.
    for(uint8_t iii = 0; iii < ucLen; i++)
    {
        pxCompareValue[iii]
        .
        .
        .
    }

    return pdTRUE; // Signaling to delete the redundant task.
}

// Function that creates a task.
void vOtherFunction( void )
{
static uint8_t ucParameterToPass;
TaskHandle_t xHandle = NULL;

    // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
    // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
    // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
    // the new task attempts to access it.
    xTaskCreateReplicated( vTaskCode, "NAME", STACK_SIZE, &ucParameterToPass, tskIDLE_PRIORITY, &xHandle, taskREPLICATED_RECOVERY, ucCompareErrorCb );
    configASSERT( xHandle );

    // Use the handle to delete the task.
    if( xHandle != NULL )
    {
        vTaskDelete( xHandle );
    }
}
```
---
<a name="cmd_xTaskSetCompareValue"></a>
``` C
void xTaskSetCompareValue( CompareValue_t xNewCompareValue )
```

Sets the compare value. Compare value is used with replicated tasks. They
are used in vTaskSyncAndCompare function for figuring if there is a
difference between the tied task executions.
Input parameters:
- xNewCompareValue - New compare value to set.
---
<a name="cmd_vTaskSyncAndCompare"></a>
``` C
void vTaskSyncAndCompare( const CompareValue_t * const pxNewCompareValue )
```

Waits until every replicated task is finished. When every task is finished
function compares the compare values and if there is a mismatch it calls the
predefined callback.

- Warning - Shall only be used for replicated tasks.

Input parameters:

- pxNewCompareValue - Pointer of the compare value to be copied from. If
NULL is passed in, previous compare value is used.

Example usage:
``` C
void vReplicatedTask( void * pvParameters )
{
    for( ;; )
    {
        // Task code goes here.

        vTaskSyncAndCompare(&xCompareValue);
    }
}
```

## General added functions
* [eTaskGetType](#cmd_eTaskGetType) : Returns the type of the input task.

<a name="cmd_eTaskGetType"></a>
``` C
eTaskType eTaskGetType( TaskHandle_t pxTaskHandle )
```
Get the type of the task.

Input parameters:

- pxTaskHandle - Handle of the task to be queried.  Passing a NULL
handle results in getting the type of calling task.
