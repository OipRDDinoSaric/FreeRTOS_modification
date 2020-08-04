/**
 * @file tests.h
 *
 * @brief   Tests for added new FreeRTOS functionality.
 */
#ifndef TESTS_H
#define TESTS_H

typedef enum
{
    TEST_PASS = 0,
    TEST_FAIL
} test_status_t;

/**
 * @brief Executes the tests for added FreeRTOS functionality.
 */
void tests_task(void * unused);

/**
 * @brief Starts special test that requires only one timed task.
 */
void tests_single_timed();

#endif /* TESTS_H */
/****END OF FILE****/
