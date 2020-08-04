/**
 * @file example.h
 *
 * @brief   Example of usage of added FreeRTOS functionality.
 */
#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifndef NDEBUG
#   define TEST_MODE 0 /*!< Determines if written tests will run. */
#else
#endif

#if 1 == TEST_MODE
#   define TIMED_TEST_SINGLE_TIMED 0 /*!< Special test that requires only one
                                          timed task. */
#else
#   define EXAMPLE_DEFAULT    1
#   define EXAMPLE_TIMED      1
#   define EXAMPLE_REPLICATED 0
#endif

#define LED_GREEN_Pin        GPIO_PIN_12
#define LED_GREEN_GPIO_Port  GPIOD
#define LED_ORANGE_Pin       GPIO_PIN_13
#define LED_ORANGE_GPIO_Port GPIOD
#define LED_RED_Pin          GPIO_PIN_14
#define LED_RED_GPIO_Port    GPIOD
#define LED_BLUE_Pin         GPIO_PIN_15
#define LED_BLUE_GPIO_Port   GPIOD

#define BTN_BLUE_Pin         GPIO_PIN_0
#define BTN_BLUE_GPIO_Port   GPIOA

void example_run(void);

#endif /* EXAMPLE_H */
/****END OF FILE****/
