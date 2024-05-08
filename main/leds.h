#ifndef LEDS_H_
#define LEDS_H_

#include <stdbool.h>
#include "driver/gpio.h"

typedef enum led_t {
  LED_RED,
  LED_BLUE
} led_t;

#define LED_BLUE_GPIO GPIO_NUM_2
#define LED_RED_GPIO GPIO_NUM_0

#define LED_HIGH 0
#define LED_LOW 1

void leds_init();
void leds_set_level(led_t led, bool level);

#endif  // LEDS_H_