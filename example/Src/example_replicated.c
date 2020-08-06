/**
 * @file example_replicated.c
 *
 * @brief   Example of usage of added replicated tasks functionality.
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
#include <semphr.h>

#include <ndebug_printf.h>

/******************************************************************************/
/* Acronym list
 *
 * Redun  - redundant
 * Revoc  - recoverable
 * Nrecov - non-recoverable
 */

#define PRIORITY_REDUN_RECOV_OK       7
#define PRIORITY_REDUN_RECOV_FAIL     7
#define PRIORITY_REDUN_NRECOV_OK      7
#define PRIORITY_REDUN_NRECOV_FAIL    7

/******************************************************************************/

TaskHandle_t h_task_redun_recov_ok       = NULL;
TaskHandle_t h_task_redun_recov_fail     = NULL;
TaskHandle_t h_task_redun_nrecov_ok      = NULL;
TaskHandle_t h_task_redun_nrecov_fail    = NULL;

static SemaphoreHandle_t h_mtx_offset_recov;
static SemaphoreHandle_t h_mtx_offset_nrecov;

static const CompareValue_t cmp_val = 1;

/******************************************************************************/

void task_redun_recov_ok      (void * unused);
void task_redun_recov_fail    (void * unused);
void task_redun_nrecov_ok     (void * unused);
void task_redun_nrecov_fail   (void * unused);

uint8_t task_redun_recov_ok_cb(CompareValue_t * compare_values, uint8_t len);
uint8_t task_redun_recov_fail_cb(CompareValue_t * compare_values, uint8_t len);
uint8_t task_redun_nrecov_ok_cb(CompareValue_t * compare_values, uint8_t len);
uint8_t task_redun_nrecov_fail_cb(CompareValue_t * compare_values, uint8_t len);

static void print_cmp_values_and_deduced_result(CompareValue_t * compare_values,
                                                uint8_t len);

/******************************************************************************/

void example_replicated_start(void)
{
    h_mtx_offset_recov  = xSemaphoreCreateMutex();
    h_mtx_offset_nrecov = xSemaphoreCreateMutex();

    xTaskCreateReplicated(task_redun_recov_ok,
                          "Repl recov ok",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_REDUN_RECOV_OK,
                          &h_task_redun_recov_ok,
                          taskREPLICATED_RECOVERY,
                          task_redun_recov_ok_cb
                         );

    xTaskCreateReplicated(task_redun_recov_fail,
                          "Repl recov fail",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_REDUN_RECOV_FAIL,
                          &h_task_redun_recov_fail,
                          taskREPLICATED_RECOVERY,
                          task_redun_recov_fail_cb
                         );

    xTaskCreateReplicated(task_redun_nrecov_ok,
                          "Repl non-recov ok",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_REDUN_RECOV_OK,
                          &h_task_redun_nrecov_ok,
                          taskREPLICATED_NO_RECOVERY,
                          task_redun_nrecov_ok_cb
                         );

    xTaskCreateReplicated(task_redun_nrecov_fail,
                          "Repl non-recov fail",
                          configMINIMAL_STACK_SIZE,
                          NULL,
                          PRIORITY_REDUN_NRECOV_FAIL,
                          &h_task_redun_nrecov_fail,
                          taskREPLICATED_NO_RECOVERY,
                          task_redun_nrecov_fail_cb
                         );
}

/******************************************************************************/

uint8_t task_redun_recov_ok_cb(CompareValue_t * compare_values, uint8_t len)
{
    ndebug_printf_w_ticks("Task \"Repl recov ok\" is not OK\n"
                          "Task \"Repl recov ok\" deleting itself\n");
    return pdTRUE;
}

void task_redun_recov_ok(void * unused)
{
    while(true)
    {
        ndebug_printf_w_ticks("Task \"%s\" is waiting for comparison.\n",
                              pcTaskGetName(NULL));

        vTaskSyncAndCompare(&cmp_val);
        vTaskDelay(pdMS_TO_TICKS(5 * 1000));
    }
}

/******************************************************************************/

uint8_t task_redun_recov_fail_cb(CompareValue_t * compare_values, uint8_t len)
{

    ndebug_printf_lock(portMAX_DELAY);

    ndebug_printf_w_ticks("Task \"Repl recov fail\" compare failed.\n");

    print_cmp_values_and_deduced_result(compare_values, len);

    ndebug_printf_unlock();

    return pdFALSE;
}

static CompareValue_t recov_offset = 1;

void task_redun_recov_fail(void * unused)
{
    vTaskDelay(pdMS_TO_TICKS(1 * 1000));

    while(true)
    {
        if(xSemaphoreTakeRecursive(h_mtx_offset_recov, portMAX_DELAY) == pdTRUE)
        {
            CompareValue_t cmp_val_fail = recov_offset;

            recov_offset = recov_offset == 1 ? 0: 1;

            xSemaphoreGiveRecursive(h_mtx_offset_recov);

            ndebug_printf_w_ticks("Task \"%s\" is waiting for comparison.\n",
                                  pcTaskGetName(NULL));

            vTaskSyncAndCompare(&cmp_val_fail);
            vTaskDelay(pdMS_TO_TICKS(5 * 1000));
        }
    }
}

/******************************************************************************/

uint8_t task_redun_nrecov_ok_cb(CompareValue_t * compare_values, uint8_t len)
{
    ndebug_printf_w_ticks("Task \"Repl non-recov ok\" is not OK\n"
                          "Task \"Repl non-recov ok\" deleting itself\n");
    return pdTRUE;
}

void task_redun_nrecov_ok(void * unused)
{
    vTaskDelay(pdMS_TO_TICKS(2 * 1000));
    while(true)
    {
        ndebug_printf_w_ticks("Task \"%s\" is waiting for comparison.\n",
                              pcTaskGetName(NULL));

        vTaskSyncAndCompare(&cmp_val);
        vTaskDelay(pdMS_TO_TICKS(5 * 1000));
    }
}

/******************************************************************************/

uint8_t task_redun_nrecov_fail_cb(CompareValue_t * compare_values, uint8_t len)
{
    ndebug_printf_lock(portMAX_DELAY);

    ndebug_printf_w_ticks("Task \"Repl non-recov fail\" compare failed.\n");

    print_cmp_values_and_deduced_result(compare_values, len);

    ndebug_printf_unlock();

    return pdFALSE;
}

static CompareValue_t nrecov_offset = 1;

void task_redun_nrecov_fail(void * unused)
{
    vTaskDelay(pdMS_TO_TICKS(1 * 1000));

    while(true)
    {
        if(xSemaphoreTakeRecursive(h_mtx_offset_nrecov, portMAX_DELAY)
           == pdTRUE)
        {
            CompareValue_t cmp_val_fail = nrecov_offset;

            nrecov_offset = nrecov_offset == 1 ? 0: 1;

            xSemaphoreGiveRecursive(h_mtx_offset_nrecov);

            ndebug_printf_w_ticks("Task \"%s\" is waiting for comparison.\n",
                                  pcTaskGetName(NULL));

            vTaskSyncAndCompare(&cmp_val_fail);
            vTaskDelay(pdMS_TO_TICKS(5 * 1000));
        }
    }
}
/******************************************************************************/

static void print_cmp_values_and_deduced_result(CompareValue_t * compare_values,
                                                uint8_t len)
{
    ndebug_printf_w_ticks("Compare values are: ");
    for(uint8_t iii = 0; iii < len; iii++)
    {
        ndebug_printf_w_ticks("%lu ", compare_values[iii]);
    }
    ndebug_printf_w_ticks("\n");

    CompareValue_t result;
    bool           is_recoverable = false;

    for(uint8_t iii = 0; iii < len; iii++)
    {
        for(uint8_t jjj = iii + 1; jjj < len; jjj++)
        {
            if(compare_values[iii] == compare_values[jjj])
            {
                result = compare_values[iii];
                is_recoverable = true;
                goto print_result;
            }
        }
    }

print_result:
    if(is_recoverable)
    {
        ndebug_printf_w_ticks("Deduced result is %lu.\n", result);
    }
    else
    {
        ndebug_printf_w_ticks("Result could not be deduced.\n");
    }
}
/****END OF FILE****/
