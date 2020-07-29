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
#include <timers.h>

#include "example.h"
#include "tests.h"

#define PRIORITY_TESTS 1

#ifndef NDEBUG
/* Used for run time stats of heap */
static const uint8_t freeRTOSMemoryScheme = 4;
#endif

void example_run (void)
{
#if  1 == TEST_MODE

    xTaskCreate(tests_task,
                "Tests",
                configMINIMAL_STACK_SIZE,
                NULL,
                PRIORITY_TESTS,
                NULL);
#else

#endif

    /* Give control to FreeRTOS */
    vTaskStartScheduler();
    for(;;);

#   ifndef NDEBUG
    (void)freeRTOSMemoryScheme;
#   endif
}

/****END OF FILE****/
