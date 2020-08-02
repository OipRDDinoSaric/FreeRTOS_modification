/**
 * @file ndebug_printf.h
 *
 * @brief   Enable debug printing. Thread safe, built on top of FreeRTOS.
 */

#include "ndebug_printf.h"

#include <stdarg.h>

#include <usart.h>

#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#define PRINTF_UART_HANDLE huart2

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

/******************************************************************************/

static void init_if_needed(void);

/******************************************************************************/

bool ndebug_printf_lock(TickType_t block_time)
{
#ifndef NDEBUG
    init_if_needed();
    return xSemaphoreTakeRecursive(mutex_freertos_atomic, block_time)
                                   == pdTRUE ? true: false;
#else
    return false;
#endif
}

/******************************************************************************/

bool ndebug_printf_unlock()
{
#ifndef NDEBUG
    init_if_needed();
    return xSemaphoreGiveRecursive(mutex_freertos_atomic) == pdTRUE ? true: false;
#else
    return false;
#endif
}

/******************************************************************************/

int	ndebug_printf(const char *format, ...)
{
#ifndef NDEBUG

    /**
     * You might be wondering, why don't we just call the ndebug_printf_try...
     * That doesn't work as it receives ..., and not va_list.
     */

    if(ndebug_printf_lock(portMAX_DELAY))
    {
        va_list args;
        va_start(args, format);
        int retval = vprintf(format, args);
        va_end(args);
        ndebug_printf_unlock();
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


/******************************************************************************/

int ndebug_printf_try(TickType_t block_time, const char *format, ...)
{
#ifndef NDEBUG

    if(ndebug_printf_lock(block_time))
    {
        va_list args;
        va_start(args, format);
        int retval = vprintf(format, args);
        va_end(args);
        ndebug_printf_unlock();
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

/******************************************************************************/

static void init_if_needed(void)
{
    static bool is_inited = false;

    if(!is_inited)
    {
        is_inited = true;
        mutex_freertos_atomic = xSemaphoreCreateMutex();
        xSemaphoreGive(mutex_freertos_atomic);
    }
}

/****END OF FILE****/
