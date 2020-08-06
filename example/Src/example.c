/**
 * @file example.c
 *
 * @brief   Example of usage of added FreeRTOS functionality.
 *
 * @note    Written according to BARR-C:2018 coding standard
 *          Exceptions:
 *              - 3.2 a, c  - Eclipse formatter doesn't support
 *              - 6.3 b iv. - ERR_CHECK() macro has return keyword
 *              - 7.1 m     - Boolean begins with is, e.g. is_example
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <ndebug_printf.h>

#include <stm32f4xx_hal.h>

#include <FreeRTOS.h>
#include <task.h>

#include "example.h"
#include "example_timed.h"
#include "example_replicated.h"

#include "tests.h"

/******************************************************************************/

#if 1 == TEST_MODE
#define PRIORITY_TESTS 2
#else
#define PRIORITY_DEFAULT              5
#endif
#ifndef NDEBUG
/* Used for run time stats of heap */
static const uint8_t freeRTOSMemoryScheme = 4;
#endif

/******************************************************************************/

TaskHandle_t h_task_default = NULL;

/******************************************************************************/

void task_default             (void * unused);

/******************************************************************************/

void example_run (void)
{
#if  1 == TEST_MODE
#if 1 == TIMED_TEST_SINGLE_TIMED
    tests_single_timed();
#else
    xTaskCreate(tests_task,
                "Tests",
                configMINIMAL_STACK_SIZE,
                NULL,
                PRIORITY_TESTS,
                NULL);
#endif
#else
#if 1 == EXAMPLE_DEFAULT
    xTaskCreate(task_default,
                "Default task",
                configMINIMAL_STACK_SIZE,
                NULL,
                PRIORITY_DEFAULT,
                &h_task_default);
#endif

#if 1 == EXAMPLE_TIMED
    example_timed_start();
#endif

#if 1 == EXAMPLE_REPLICATED
    example_replicated_start();
#endif
#endif

    /* Give control to FreeRTOS */
    vTaskStartScheduler();
    for(;;);

#   ifndef NDEBUG
    (void)freeRTOSMemoryScheme;
#   endif
}

/******************************************************************************/


void task_default(void * unused)
{
    while(true)
    {
        ndebug_printf("Task \"%s\" says hello world!\n", pcTaskGetName(NULL));
        vTaskDelay(pdMS_TO_TICKS(5 * 1000));
    }
}

/******************************************************************************/


/****END OF FILE****/
