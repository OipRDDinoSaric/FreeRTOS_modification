/**
 * @file FreeRTOS_run_time_stats_timer.h
 *
 * @brief   Initialization of timer for run time statistics when debugging with
 *          FreeRTOS
 */
#ifndef FREERTOS_RUN_TIME_STATS_TIMER_H
#define FREERTOS_RUN_TIME_STATS_TIMER_H
#include "stdint.h"


void init_run_time_timer(void);
uint32_t get_run_time_count (void);

#endif /* FREERTOS_RUN_TIME_STATS_TIMER_H */
/****END OF FILE****/

