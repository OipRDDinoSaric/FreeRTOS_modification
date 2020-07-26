/**
 * @file ndebug_printf.c
 *
 * @brief   Enable debug printing.
 *
 */

#include <stdio.h>
#include <stdarg.h>

#include <usart.h>

#include "ndebug_printf.h"

#ifndef NDEBUG

#define PRINTF_UART_HANDLE huart2

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
	va_start(args, format);
	int retval = vprintf(format, args);
	va_end(args);
	return retval;
}

#else

#define	ndebug_printf(ignore, ...) ((void)0u)

#endif

/****END OF FILE****/
