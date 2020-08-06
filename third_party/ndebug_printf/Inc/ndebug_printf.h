/**
 * @file ndebug_printf.h
 *
 * @brief   Enable debug printing. Thread safe, built on top of FreeRTOS.
 */
#ifndef NDEBUG_PRINTF_H
#define NDEBUG_PRINTF_H

#include <stdbool.h>
#include <FreeRTOS.h>

/**
 * @brief Locks the printf so other tasks can not print while calling is
 *
 * @note When NDEBUG is defined returns false.
 *
 * @param block_time [in] Time to block the task when locking the mutex.
 *
 * @return true on success, false on fail.
 */
bool ndebug_printf_lock(TickType_t block_time);

/**
 * @brief Unlocks the printf so other tasks can print.
 *
 * @note When NDEBUG is defined returns false.
 *
 * @return true on success, false on fail.
 */
bool ndebug_printf_unlock();

/**
 * @brief Thread safe printf for debugging.
 *
 * @note When NDEBUG is defined it ignores the message.
 *
 * @return If successful, the total number of characters written is returned.
 *         On failure, a negative number is returned.
 */
int	ndebug_printf(const char *format, ...);

/**
 * @brief Thread safe printf for debugging, adds current tick to the front.
 *
 * @note When NDEBUG is defined it ignores the message.
 *
 * @return If successful, the total number of characters written is returned.
 *         On failure, a negative number is returned.
 */
int ndebug_printf_w_ticks(const char *format, ...);
/**
 * @brief Thread safe printf for debugging.
 *
 * @note When NDEBUG is defined it ignores the message.
 *
 * @param block_time [in] Time to block the task when printing.
 *
 * @return If successful, the total number of characters written is returned.
 *         On failure, a negative number is returned.
 */
int ndebug_printf_try(TickType_t block_time, const char *format, ...);

#endif /* NDEBUG_PRINTF_H */
/****END OF FILE****/
