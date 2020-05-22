/**
 * @file FreeRTOS_run_time_stats_timer.c
 *
 * @brief   Initialization of timer for run time statistics when debugging with
 *          FreeRTOS
 *
 * @note    Written according to BARR-C:2018 coding standard
 *          Exceptions:
 *              - 3.2 a, c  - Eclipse formatter doesn't support
 *              - 6.3 b iv. - ERR_CHECK() macro has return keyword
 *              - 7.1 m     - Boolean begins with is, e.g. is_example
 */

#include "FreeRTOS_run_time_stats_timer.h"
#include "tim.h"

#define ph_stats_timer &htim2

#define INIT_RUN_TIME_STATS_TIMER(name) MX_##name##_Init()

void init_run_time_timer (void)
{
    INIT_RUN_TIME_STATS_TIMER(TIM2);

    HAL_TIM_Base_Start(ph_stats_timer);
}

uint32_t get_run_time_count (void)
{
    return __HAL_TIM_GET_COUNTER(ph_stats_timer);
}

/****END OF FILE****/
