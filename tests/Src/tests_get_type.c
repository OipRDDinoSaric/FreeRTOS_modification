/**
 * @file tests_get_type.c
 *
 * @brief   Tests for added get type of the task.
 */

#include "tests_get_type.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stdlib.h>

/******************************************************************************/

static test_status_t check_task_types(TaskHandle_t h_default,
                                      TaskHandle_t h_timed,
                                      TaskHandle_t h_replicated);

/******************************************************************************/

static void delete_test_tasks(TaskHandle_t h_default,
                              TaskHandle_t h_timed,
                              TaskHandle_t h_replicated);

/******************************************************************************/

#undef TEST_TASK_PRIORITY
#define TEST_TASK_PRIORITY 3

#define TIMED_TIMEOUT 2 * 1000 /* Enough to test type. */

/******************************************************************************/

void    task_generic    (void * unused);
uint8_t compare_fail_cb (CompareValue_t * compare_values, uint8_t len);
void    timed_timeout_cb(WorstTimeTimerHandle_t h_timer);

/******************************************************************************/

/**
 * @brief   Tests for added get type of the task.
 */
test_status_t tests_get_type(void)
{
    TaskHandle_t h_default = NULL;
    TaskHandle_t h_timed = NULL;
    TaskHandle_t h_replicated = NULL;

#if 1
    if(xTaskCreate(task_generic,
                   "default",
                   configMINIMAL_STACK_SIZE,
                   NULL,
                   TEST_TASK_PRIORITY,
                   &h_default) != pdPASS)
    {
        return TEST_FAIL;
    }
#endif
#if 1
    if(xTaskCreateTimed(task_generic,
                        "timed",
                        configMINIMAL_STACK_SIZE,
                        NULL,
                        TEST_TASK_PRIORITY,
                        &h_timed,
                        0,
                        NULL,
                        TIMED_TIMEOUT,
                        timed_timeout_cb
                       ) != pdPASS)
    {
        return TEST_FAIL;
    }
#endif
#if 1
    if(xTaskCreateReplicated(task_generic,
                             "replicated",
                             configMINIMAL_STACK_SIZE,
                             NULL,
                             TEST_TASK_PRIORITY,
                             &h_replicated,
                             taskREPLICATED_RECOVERY,
                             compare_fail_cb
                            ) != pdPASS)
    {
        return TEST_FAIL;
    }
#endif

    test_status_t test_status = check_task_types(h_default,
                                                 h_timed,
                                                 h_replicated);

    delete_test_tasks(h_default, h_timed, h_replicated);

    return test_status;
}

/******************************************************************************/

static test_status_t check_task_types(TaskHandle_t h_default,
                                      TaskHandle_t h_timed,
                                      TaskHandle_t h_replicated)
{
    if(eTaskGetType(h_timed) != eTimed)
    {
        return TEST_FAIL;
    }

    if(eTaskGetType(h_default) != eDefault)
    {
        return TEST_FAIL;
    }

    if(eTaskGetType(h_replicated) != eReplicated)
    {
        return TEST_FAIL;
    }

    return TEST_PASS;
}

/******************************************************************************/

static void delete_test_tasks(TaskHandle_t h_default,
                              TaskHandle_t h_timed,
                              TaskHandle_t h_replicated)
{
    if(h_default != NULL)
    {
        vTaskDelete(h_default);
    }

    if(h_timed != NULL)
    {
        vTaskDelete(h_timed);
    }

    if(h_replicated != NULL)
    {
        vTaskDelete(h_replicated);
    }
}

/******************************************************************************/

void task_generic(void * unused)
{
    vTaskDelay(portMAX_DELAY);
}

/******************************************************************************/

uint8_t compare_fail_cb(CompareValue_t * compare_values, uint8_t len)
{
    /* Never called. */
    return pdTRUE;
}

/******************************************************************************/


void timed_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    /* Never called. */
}

/******************************************************************************/

/****END OF FILE****/
