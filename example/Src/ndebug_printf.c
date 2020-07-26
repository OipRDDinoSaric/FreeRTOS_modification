/**
 * @file ndebug_printf.c
 *
 * @brief   Enable debug printing.
 *
 */

#include "ndebug_printf.h"

#ifndef NDEBUG

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

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


int	ndebug_printf(const char *format, ...)
{
	va_list args;

	if(!isInited)
	{
	    isInited = true;
	    mutex_freertos_atomic = xSemaphoreCreateMutex();
        xSemaphoreGive(mutex_freertos_atomic);
	}

	if(xSemaphoreTake(mutex_freertos_atomic, pdMS_TO_TICKS(2 * 1000)) == pdTRUE)
	{
        va_start(args, format);
        int retval = vprintf(format, args);
        va_end(args);
        xSemaphoreGive(mutex_freertos_atomic);
        return retval;
	}
	else
	{
	    return -1;
	}
}

#else

#define	ndebug_printf(ignore, ...) ((void)0u)

#endif

/****END OF FILE****/
