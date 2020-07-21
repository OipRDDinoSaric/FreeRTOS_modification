/**
 * @file use_example.c
 *
 * @brief   Tests for added new FreeRTOS functionality.
 */

/*----------------------------------------------------------------------------*/

#include <stdio.h>

#include <FreeRTOS.h>
#include <semphr.h>

#include "tests.h"

/*----------------------------------------------------------------------------*/

typedef enum
{
    TEST_PASS = 0,
    TEST_FAIL
} test_status_t;

/*----------------------------------------------------------------------------*/

static test_status_t tests_timed_task();

/*----------------------------------------------------------------------------*/

static test_status_t tests_replicated_task();

/*----------------------------------------------------------------------------*/

static test_status_t tests_get_type();

/*----------------------------------------------------------------------------*/

void tests_run(void)
{
    test_status_t test_status;

    printf("Testing timed tasks...");
    test_status = tests_timed_task();
    printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");

    printf("Testing replicated tasks...");
    test_status = tests_replicated_task();
    printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");

    printf("Testing get task type...");
    test_status = tests_get_type();
    printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");
}

/*----------------------------------------------------------------------------*/

static test_status_t tests_timed_task()
{
    test_status_t test_status = TEST_FAIL;

    return test_status;
}

/*----------------------------------------------------------------------------*/

static test_status_t tests_replicated_task()
{
    test_status_t test_status = TEST_FAIL;

    return test_status;
}

/*----------------------------------------------------------------------------*/

#undef PRIORITY_DAEMON
#undef PRIORITY_TEST
#define PRIORITY_DAEMON 5
#define PRIORITY_TEST   4

static void get_type_daemon(test_status_t * test_status);
static void get_type_task_timed(void);
static void get_type_task_replicated(void);
static void get_type_task_default(void);

static SemaphoreHandle_t g_smph_test_done;

static test_status_t tests_get_type()
{
    test_status_t test_status = TEST_PASS;

    g_smph_test_done = xSemaphoreCreateBinary(); /* Not takeable. */

    xTaskCreate(get_type_daemon,
                "Type daemon",
                configMINIMAL_STACK_SIZE,
                &test_status, /* Will be available! */
                PRIORITY_DAEMON,
                NULL);

    xSemaphoreTake(g_smph_test_done, portMAX_DELAY);

    return test_status;
}

static void get_type_daemon(test_status_t * test_status)
{
    g_smph_subtest_done = xSemaphoreCreateBinary(); /* Not takeable. */

    /* Create timed, replicated and default task with lower priority than daemon. */
    /* Check their type. */
}



/*----------------------------------------------------------------------------*/

/****END OF FILE****/
