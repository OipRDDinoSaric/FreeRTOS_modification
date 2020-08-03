/**
 * @file tests_timed_task.c
 *
 * @brief   Tests for added timed task.
 */

#include "tests_timed_task.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <stdbool.h>

#include "ndebug_printf.h"

/******************************************************************************/

#undef TEST_TASK_PRIORITY
#define TEST_TASK_PRIORITY 5

#undef TIMED_TIMEOUT
#define TIMED_TIMEOUT 500


/******************************************************************************/

test_status_t test_slower_than_timeout();
test_status_t test_faster_than_timeout();
void task_slower(void * unused);
void task_faster(void * unused);
void slower_timeout_cb(WorstTimeTimerHandle_t h_timer);
void faster_timeout_cb(WorstTimeTimerHandle_t h_timer);

/******************************************************************************/

static volatile bool isSuccess;

/******************************************************************************/

void tests_timed_task(void)
{
    ndebug_printf("   Test slower than timeout: %s\n",
                  test_slower_than_timeout() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("    Test faster than timeout: %s\n",
                  test_slower_than_timeout() == TEST_PASS?
                  "OK": "FAIL");

}

/******************************************************************************/

test_status_t test_slower_than_timeout()
{
    TaskHandle_t h_timed = NULL;

    isSuccess = false;

    xTaskCreateTimed(task_slower,
                     "slower",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     0,
                     NULL,
                     TIMED_TIMEOUT,
                     slower_timeout_cb);

    vTaskDelay(TIMED_TIMEOUT + 100);

    if(h_timed)
    {
        vTaskDelete(h_timed);
    }

    return isSuccess ? TEST_PASS : TEST_FAIL;
}

/******************************************************************************/

void task_slower(void * unused)
{
    vTaskDelay(portMAX_DELAY);
}

/******************************************************************************/

void slower_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    isSuccess = true;
}

/******************************************************************************/

test_status_t test_faster_than_timeout()
{
    TaskHandle_t h_timed = NULL;

    isSuccess = true;

    xTaskCreateTimed(task_faster,
                     "slower",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     0,
                     NULL,
                     TIMED_TIMEOUT,
                     faster_timeout_cb
                    );

    vTaskDelay(TIMED_TIMEOUT + 100);

    if(h_timed)
    {
        ndebug_printf("Deleting faster timed task.\n");
        vTaskDelete(h_timed);
    }

    return isSuccess ? TEST_PASS : TEST_FAIL;
    return TEST_PASS;
}

/******************************************************************************/

void task_faster(void * unused)
{
    while(true)
    {
        ndebug_printf("Reseting faster task.\n");
        vTaskTimedReset(NULL);
        vTaskDelay(TIMED_TIMEOUT - 100);
    }
}

/******************************************************************************/

void faster_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    ndebug_printf("Faster timeout triggered.\n");
    isSuccess = false;
}

/******************************************************************************/
/****END OF FILE****/
