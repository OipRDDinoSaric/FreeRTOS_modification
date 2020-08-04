/**
 * @file tests_timer_additions.c
 *
 * @brief   Tests for timer additions.
 */
#include "tests_timer_additions.h"

#include <FreeRTOS.h>
#include <timers.h>

#include <stdbool.h>

#include <ndebug_printf.h>

/******************************************************************************/

TimerHandle_t h_timer = NULL;

static bool is_cb_triggered = false;

/******************************************************************************/

void timer_cb(TimerHandle_t h_timer)
{
    is_cb_triggered = true;
}

/******************************************************************************/

static test_status_t test_start            (void);
static test_status_t test_pause_resume     (void);
static test_status_t test_pause_stop_resume(void);
static test_status_t test_pause_start      (void);

/******************************************************************************/

void tests_timer_add_task(void)
{
    h_timer = xTimerCreate("Test timer",
                           pdMS_TO_TICKS(1000),
                           pdTRUE, NULL,
                           timer_cb);
    if(h_timer == NULL)
    {
        ndebug_printf("Can't create test timer.");
        return;
    }
    ndebug_printf("   Start...%s\n",
                  test_start() == TEST_PASS?
                  "OK": "FAIL");
    ndebug_printf("   Pause, stop, resume..%s\n",
                  test_pause_stop_resume() == TEST_PASS?
                  "OK": "FAIL");
    ndebug_printf("   Pause, stop, start...%s\n",
                  test_pause_start() == TEST_PASS?
                  "OK": "FAIL");
    ndebug_printf("   Pause, resume...%s\n",
                  test_pause_resume() == TEST_PASS?
                  "OK": "FAIL");

    xTimerDelete(h_timer, portMAX_DELAY);

}

/******************************************************************************/

static test_status_t test_start(void)
{
    is_cb_triggered = false;
    xTimerStart(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(1200));

    if(is_cb_triggered)
    {
        return TEST_PASS;
    }

    return TEST_FAIL;
}

/******************************************************************************/

static test_status_t test_pause_resume(void)
{
    is_cb_triggered = false;
    xTimerStart(h_timer, portMAX_DELAY); /* Resets the timer to initial state. */

    vTaskDelay(pdMS_TO_TICKS(800));

    xTimerPause(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(500));

    if(is_cb_triggered)
    {
        return TEST_FAIL;
    }

    xTimerResume(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(300));

    if(is_cb_triggered)
    {
        return TEST_PASS;
    }

    return TEST_FAIL;
}

/******************************************************************************/

static test_status_t test_pause_stop_resume(void)
{
    is_cb_triggered = false;
    xTimerStart(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(800));

    xTimerPause(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(500));

    xTimerStop(h_timer, portMAX_DELAY);

    xTimerResume(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(210));

    if(is_cb_triggered)
    {
        return TEST_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(800));

    if(is_cb_triggered)
    {
        return TEST_PASS;
    }

    return TEST_FAIL;
}

/******************************************************************************/

static test_status_t test_pause_start(void)
{
    is_cb_triggered = false;
    xTimerStart(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(800));

    xTimerPause(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(500));

    xTimerStart(h_timer, portMAX_DELAY);

    vTaskDelay(pdMS_TO_TICKS(210));

    if(is_cb_triggered)
    {
        return TEST_FAIL;
    }

    vTaskDelay(pdMS_TO_TICKS(800));

    if(is_cb_triggered)
    {
        return TEST_PASS;
    }

    return TEST_FAIL;
}

/****END OF FILE****/
