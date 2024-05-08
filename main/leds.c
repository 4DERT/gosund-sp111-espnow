#include "leds.h"

#include "stdbool.h"

void leds_init() {
  // BLUE LED
  gpio_config_t led_blue_cfg;
  led_blue_cfg.pin_bit_mask = (1UL << LED_BLUE_GPIO);
  led_blue_cfg.mode = GPIO_MODE_OUTPUT;
  led_blue_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_blue_cfg);
  gpio_set_level(LED_BLUE_GPIO, LED_LOW);

  // RED LED
  gpio_config_t led_red_cfg;
  led_red_cfg.pin_bit_mask = (1UL << LED_RED_GPIO);
  led_red_cfg.mode = GPIO_MODE_OUTPUT;
  led_red_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_red_cfg);
  gpio_set_level(LED_RED_GPIO, LED_LOW);
}

void leds_set_level(led_t led, bool level) {
  switch (led) {
    case LED_RED:
      gpio_set_level(LED_RED_GPIO, level);
      break;

    case LED_BLUE:
      gpio_set_level(LED_BLUE_GPIO, level);
      break;

    default:
      break;
  }
}