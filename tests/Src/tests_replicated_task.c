/**
 * @file tests_replicated_task.c
 *
 * @brief   Tests for added replicated task.
 */

#include "tests_replicated_task.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stdbool.h>

#include <ndebug_printf.h>

/******************************************************************************/

#undef PRIORITY_TEST
#define PRIORITY_TEST 4

/******************************************************************************/

static bool isSuccess = true;

/******************************************************************************/

test_status_t test_shall_not_call_callback(void);
test_status_t test_shall_not_call_recovery_callback(void);
test_status_t test_shall_call_callback(void);
test_status_t test_shall_call_recovery_callback(void);
test_status_t test_shall_not_call_recovery_callback_setCompare(void);
test_status_t test_shall_call_recovery_callback_setCompare(void);
test_status_t test_deleting_replicated(void);

/******************************************************************************/

void tests_replicated_task(void)
{
    ndebug_printf("   Shall not call callback...%s\n",
                  test_shall_not_call_callback() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Shall not call callback, with recovery...%s\n",
                  test_shall_not_call_recovery_callback() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Shall call callback...%s\n",
                  test_shall_call_callback() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Shall call callback, with recovery...%s\n",
                  test_shall_call_recovery_callback() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Shall not call callback, with recovery, using vTaskSetCompareValue...%s\n",
                  test_shall_not_call_recovery_callback_setCompare() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Shall call callback, with recovery, using vTaskSetCompareValue...%s\n",
                  test_shall_call_recovery_callback_setCompare() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("   Deleting non recoverable and recoverable test...%s\n",
                  test_deleting_replicated() == TEST_PASS?
                  "OK": "FAIL");
}

/******************************************************************************/

uint8_t shall_not_call_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_not_call(void * unused);

test_status_t test_shall_not_call_callback(void)
{
    TaskHandle_t h_shall_not_call = NULL;
    xTaskCreateReplicated(task_shall_not_call,
                          "Not call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_not_call,
                          taskREPLICATED_NO_RECOVERY,
                          shall_not_call_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_not_call != NULL)
    {
        vTaskDelete(h_shall_not_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static const CompareValue_t shall_not_call_value = 10;

static void task_shall_not_call(void * unused)
{
    while(true)
    {
        vTaskSyncAndCompare(&shall_not_call_value);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_not_call_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = false;

    return pdTRUE;
}

/******************************************************************************/

uint8_t shall_not_call_recovery_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_not_call_recovery(void * unused);

test_status_t test_shall_not_call_recovery_callback(void)
{
    TaskHandle_t h_shall_not_call = NULL;
    xTaskCreateReplicated(task_shall_not_call_recovery,
                          "Not call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_not_call,
                          taskREPLICATED_RECOVERY,
                          shall_not_call_recovery_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_not_call != NULL)
    {
        vTaskDelete(h_shall_not_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static const CompareValue_t shall_not_call_value_recovery = 10;

static void task_shall_not_call_recovery(void * unused)
{
    while(true)
    {
        vTaskSyncAndCompare(&shall_not_call_value_recovery);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_not_call_recovery_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = false;

    return pdTRUE;
}

/******************************************************************************/

uint8_t shall_call_recovery_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_call_recovery(void * unused);

test_status_t test_shall_call_recovery_callback(void)
{
    TaskHandle_t h_shall_call = NULL;

    isSuccess = false;

    xTaskCreateReplicated(task_shall_call_recovery,
                          "Call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_call,
                          taskREPLICATED_RECOVERY,
                          shall_call_recovery_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_call != NULL)
    {
        vTaskDelete(h_shall_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static CompareValue_t shall_call_value_recovery = 10;

static void task_shall_call_recovery(void * unused)
{
    while(true)
    {
        shall_call_value_recovery++;
        vTaskSyncAndCompare(&shall_call_value_recovery);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_call_recovery_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = true;

    return pdFALSE;
}

/******************************************************************************/

uint8_t shall_call_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_call(void * unused);

test_status_t test_shall_call_callback(void)
{
    TaskHandle_t h_shall_call = NULL;

    isSuccess = false;

    xTaskCreateReplicated(task_shall_call,
                          "Call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_call,
                          taskREPLICATED_RECOVERY,
                          shall_call_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_call != NULL)
    {
        vTaskDelete(h_shall_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static CompareValue_t shall_call_value = 10;

static void task_shall_call(void * unused)
{
    while(true)
    {
        shall_call_value++;
        vTaskSyncAndCompare(&shall_call_value);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_call_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = true;

    return pdFALSE;
}

/******************************************************************************/

uint8_t shall_not_call_setCompare_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_not_call_setCompare(void * unused);

test_status_t test_shall_not_call_recovery_callback_setCompare(void)
{
    TaskHandle_t h_shall_call = NULL;

    isSuccess = true;

    xTaskCreateReplicated(task_shall_not_call_setCompare,
                          "Not call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_call,
                          taskREPLICATED_RECOVERY,
                          shall_not_call_setCompare_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_call != NULL)
    {
        vTaskDelete(h_shall_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static const CompareValue_t shall_not_call_setCompare_value = 10;

static void task_shall_not_call_setCompare(void * unused)
{
    while(true)
    {
        vTaskSetCompareValue(shall_not_call_setCompare_value);

        vTaskSyncAndCompare(NULL);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_not_call_setCompare_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = false;

    return pdFALSE;
}

/******************************************************************************/

uint8_t shall_call_setCompare_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_shall_call_setCompare(void * unused);

test_status_t test_shall_call_recovery_callback_setCompare(void)
{
    TaskHandle_t h_shall_call = NULL;

    isSuccess = false;

    xTaskCreateReplicated(task_shall_call_setCompare,
                          "Call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_shall_call,
                          taskREPLICATED_RECOVERY,
                          shall_call_setCompare_cb);

    vTaskDelay(pdMS_TO_TICKS(2000));

    if(h_shall_call != NULL)
    {
        vTaskDelete(h_shall_call);
    }

    return isSuccess ? TEST_PASS: TEST_FAIL;
}

static CompareValue_t shall_call_setCompare_value = 10;

static void task_shall_call_setCompare(void * unused)
{
    while(true)
    {
        shall_call_setCompare_value++;
        vTaskSetCompareValue(shall_call_setCompare_value);
        vTaskSyncAndCompare(NULL);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

uint8_t shall_call_setCompare_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    isSuccess = true;

    return pdFALSE;
}

/******************************************************************************/

uint8_t deleting_task_cb(CompareValue_t * pxCompareValues, uint8_t ucLen);
static void task_deleting_task(void * unused);

test_status_t test_deleting_replicated(void)
{
    TaskHandle_t h_replicated = NULL;

    xTaskCreateReplicated(task_deleting_task,
                          "Call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_replicated,
                          taskREPLICATED_RECOVERY,
                          deleting_task_cb);

    uint32_t num_of_tasks_recovery = uxTaskGetNumberOfTasks();

    if(h_replicated != NULL)
    {
        vTaskDelete(h_replicated);
        h_replicated = NULL;
    }

    if(uxTaskGetNumberOfTasks() != (num_of_tasks_recovery - 3))
    {
        return TEST_FAIL;
    }

    xTaskCreateReplicated(task_deleting_task,
                          "Call",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_TEST,
                          &h_replicated,
                          taskREPLICATED_NO_RECOVERY,
                          deleting_task_cb);

    uint32_t num_of_tasks_no_recovery = uxTaskGetNumberOfTasks();

    if(h_replicated != NULL)
    {
        vTaskDelete(h_replicated);
    }

    if(uxTaskGetNumberOfTasks() != (num_of_tasks_no_recovery - 2))
    {
        return TEST_FAIL;
    }

    return TEST_PASS;
}

static void task_deleting_task(void * unused)
{
    while(true)
    {
        vTaskDelay(portMAX_DELAY);
    }
}

uint8_t deleting_task_cb(CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    return pdFALSE;
}

/******************************************************************************/
/****END OF FILE****/
