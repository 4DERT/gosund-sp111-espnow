#ifndef GOSUND_SP111_PINOUT_H_
#define GOSUND_SP111_PINOUT_H_

#include "driver/gpio.h"

#define PINOUT_BUTTON_GPIO GPIO_NUM_13
#define PINOUT_RELAY_GPIO GPIO_NUM_15
#define PINOUT_LED_BLUE_GPIO GPIO_NUM_2
#define PINOUT_LED_RED_GPIO GPIO_NUM_0
#define PINOUT_CF_GPIO GPIO_NUM_5
#define PINOUT_CF1_GPIO GPIO_NUM_4
#define PINOUT_SEL_GPIO GPIO_NUM_12

#define PINOUT_LED_HIGH 0
#define PINOUT_LED_LOW 1
#define PINOUT_BUTTON_PRESSED 0

typedef void(*button_isr_handler_t)(void* arg);

void pinout_gpio_init(button_isr_handler_t button_isr_handler);

#endif //GOSUND_SP111_PINOUT_H_