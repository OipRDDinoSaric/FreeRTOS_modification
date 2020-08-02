/**
 * @file ndebug_printf.h
 *
 * @brief   Enable debug printing. Thread safe, built on top of FreeRTOS.
 */

#include "ndebug_printf.h"

#include <stdio.h>
#include <stdarg.h>

#include <usart.h>

#include <FreeRTOS.h>
#include <semphr.h>

#define PRINTF_UART_HANDLE huart2

static bool isInited = false;

static SemaphoreHandle_t mutex_freertos_atomic = NULL;

#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  HAL_UART_Transmit(&PRINTF_UART_HANDLE, (uint8_t *)&ch, 1, 0xFFFFFFFF);
  return ch;
}

bool ndebug_printf_lock(TickType_t block_time)
{
#ifndef NDEBUG
    return xSemaphoreTake(mutex_freertos_atomic, block_time)
           == pdTRUE ? true: false;
#else
    return false;
#endif
}

bool ndebug_printf_unlock()
{
#ifndef NDEBUG
    return xSemaphoreGive(mutex_freertos_atomic) == pdTRUE ? true: false;
#else
    return false;
#endif
}

int	ndebug_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    ndebug_printf_try(portMAX_DELAY, format, args);
    va_end(args);
}


int ndebug_printf_try(TickType_t block_time, const char *format, ...)
{
#ifndef NDEBUG
    va_list args;

    if(!isInited)
    {
        isInited = true;
        mutex_freertos_atomic = xSemaphoreCreateMutex();
        xSemaphoreGive(mutex_freertos_atomic);
    }

    if(xSemaphoreTake(mutex_freertos_atomic, block_time) == pdTRUE)
    {
        va_start(args, format);
        int retval = vprintf(format, args);
        va_end(args);
        return retval;
    }
    else
    {
        return -1;
    }
#else
    return -1;
#endif

}


/****END OF FILE****/
