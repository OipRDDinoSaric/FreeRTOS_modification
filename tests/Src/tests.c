/**
 * @file use_example.c
 *
 * @brief   Tests for added new FreeRTOS functionality.
 */

/*----------------------------------------------------------------------------*/

#include <ndebug_printf.h>

#include <FreeRTOS.h>
#include <task.h>

#include "tests.h"

#include "tests_timed_task.h"
#include "tests_timer_additions.h"
#include "tests_replicated_task.h"
#include "tests_get_type.h"

/*----------------------------------------------------------------------------*/

static void single_timed_task(void * unused);
void overrun_cb(WorstTimeTimerHandle_t h_timer);
void overflow_cb(WorstTimeTimerHandle_t h_timer);

/*----------------------------------------------------------------------------*/

void tests_task(void * unused)
{
    for(;;)
    {
        ndebug_printf("\n\n");
#if 1
        ndebug_printf("Testing timed tasks:\n");
        tests_timed_task();
#endif

#if 1
        ndebug_printf("Testing timer additions:\n");
        tests_timer_add_task();
#endif

#if 0
        ndebug_printf("Testing replicated tasks:\n");
        tests_replicated_task();
#endif

#if 0
        ndebug_printf("Testing get task type...");
        test_status_t test_status = tests_get_type();
        ndebug_printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");
#endif
        vTaskDelay(pdMS_TO_TICKS(2 * 1000));
    }
}

/*----------------------------------------------------------------------------*/

void tests_single_timed()
{
    xTaskCreateTimed(single_timed_task,
                     "Single timed",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     1,
                     NULL,
                     pdMS_TO_TICKS(100),
                     overrun_cb,
                     pdMS_TO_TICKS(1000),
                     overflow_cb);
    ndebug_printf("If overrun timer and overflow timer are"
                  " not triggered soon there is a problem.\n");
}

/*----------------------------------------------------------------------------*/
void overrun_cb(WorstTimeTimerHandle_t h_timer)
{
    ndebug_printf("Overrun callback triggered!\n");
}

/*----------------------------------------------------------------------------*/

void overflow_cb(WorstTimeTimerHandle_t h_timer)
{
    ndebug_printf("Overflow callback triggered!\n");
}

/*----------------------------------------------------------------------------*/

static void single_timed_task(void * unused)
{
    while(true)
    {
        vTaskDelay(portMAX_DELAY);
    }
}
/****END OF FILE****/
