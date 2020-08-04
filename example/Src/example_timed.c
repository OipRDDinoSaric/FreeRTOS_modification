/**
 * @file example_timed.c
 *
 * @brief   Example of usage of added timed tasks functionality.
 *
 * @note    Written according to BARR-C:2018 coding standard
 *          Exceptions:
 *              - 3.2 a, c  - Eclipse formatter doesn't support
 *              - 6.3 b iv. - ERR_CHECK() macro has return keyword
 *              - 7.1 m     - Boolean begins with is, e.g. is_example
 */

/******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <ndebug_printf.h>

/******************************************************************************/

/* Acronym list
 *
 * Del    - delete
 */

#define PRIORITY_TIMED_RESET_OK       3
#define PRIORITY_TIMED_RESET_FAIL     3
#define PRIORITY_TIMED_RESET_FAIL_DEL 3

#define TIMEOUT pdMS_TO_TICKS(3 * 1000)

/******************************************************************************/

TaskHandle_t h_task_timed_reset_ok       = NULL;
TaskHandle_t h_task_timed_reset_fail     = NULL;
TaskHandle_t h_task_timed_reset_fail_del = NULL;

/******************************************************************************/

void task_timed_reset_ok      (void * unused);
void task_timed_reset_fail    (void * unused);
void task_timed_reset_fail_del(void * unused);

void task_timed_reset_ok_cb      (WorstTimeTimerHandle_t timer);
void task_timed_reset_fail_cb    (WorstTimeTimerHandle_t timer);
void task_timed_reset_fail_del_cb(WorstTimeTimerHandle_t timer);

/******************************************************************************/

void example_timed_start(void)
{
    xTaskCreateTimed(task_timed_reset_ok,
                     "Timed task ok",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     PRIORITY_TIMED_RESET_OK,
                     &h_task_timed_reset_ok,
                     0,
                     NULL,
                     TIMEOUT,
                     task_timed_reset_ok_cb
                     );

    xTaskCreateTimed(task_timed_reset_fail,
                     "Timed task fail",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     PRIORITY_TIMED_RESET_FAIL,
                     &h_task_timed_reset_ok,
                     0,
                     NULL,
                     TIMEOUT,
                     task_timed_reset_fail_cb
                     );

    xTaskCreateTimed(task_timed_reset_fail_del,
                     "Timed task fail and delete",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     PRIORITY_TIMED_RESET_FAIL_DEL,
                     &h_task_timed_reset_ok,
                     0,
                     NULL,
                     TIMEOUT,
                     task_timed_reset_fail_del_cb
                     );
}

/******************************************************************************/

void task_timed_reset_ok_cb(WorstTimeTimerHandle_t timer)
{
    ndebug_printf("Task \"%s\" overflowed. This shouldn't happen.\n",
                  pcTaskGetName(xTimerGetTaskHandle(timer)));
}

void task_timed_reset_ok(void * unused)
{
    while(true)
    {
        vTaskDelay(TIMEOUT - 100);
        ndebug_printf("Task \"%s\" is reseting the timer.\n",
                      pcTaskGetName(NULL));
        vTaskTimedReset(NULL);
    }
}

/******************************************************************************/

void task_timed_reset_fail_cb(WorstTimeTimerHandle_t timer)
{
    ndebug_printf("Task \"%s\" overflowed. This happens every 3 s.\n",
                  pcTaskGetName(xTimerGetTaskHandle(timer)));
}

void task_timed_reset_fail(void * unused)
{
    vTaskDelay(pdMS_TO_TICKS(900));
    while(true)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

/******************************************************************************/

void task_timed_reset_fail_del_cb(WorstTimeTimerHandle_t timer)
{
    TaskHandle_t task_handle = xTimerGetTaskHandle(timer);

    ndebug_printf("Task \"%s\" overflowed. Deleting task.\n",
                  pcTaskGetName(task_handle));
    vTaskDelete(task_handle);
}

void task_timed_reset_fail_del(void * unused)
{
    vTaskDelay(pdMS_TO_TICKS(1900));
    while(true)
    {
        vTaskDelay(TIMEOUT + 100);
        vTaskTimedReset(NULL);
    }
}

/******************************************************************************/

/****END OF FILE****/
