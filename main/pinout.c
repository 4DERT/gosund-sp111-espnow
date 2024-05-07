#include "pinout.h"

void pinout_gpio_init() {
  // RELAY
  gpio_config_t relay_cfg;
  relay_cfg.pin_bit_mask = (1UL << PINOUT_RELAY_GPIO);
  relay_cfg.mode = GPIO_MODE_OUTPUT;
  relay_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&relay_cfg);
  gpio_set_level(PINOUT_RELAY_GPIO, PINOUT_RELAY_LOW);

  // BLUE LED
  gpio_config_t led_blue_cfg;
  led_blue_cfg.pin_bit_mask = (1UL << PINOUT_LED_BLUE_GPIO);
  led_blue_cfg.mode = GPIO_MODE_OUTPUT;
  led_blue_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_blue_cfg);
  gpio_set_level(PINOUT_LED_BLUE_GPIO, PINOUT_LED_LOW);

  // RED LED
  gpio_config_t led_red_cfg;
  led_red_cfg.pin_bit_mask = (1UL << PINOUT_LED_RED_GPIO);
  led_red_cfg.mode = GPIO_MODE_OUTPUT;
  led_red_cfg.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&led_red_cfg);
  gpio_set_level(PINOUT_LED_RED_GPIO, PINOUT_LED_LOW);

  // BUTTON
  // gpio_config_t key_cfg;
  // key_cfg.pin_bit_mask = (1UL << PINOUT_BUTTON_GPIO);
  // key_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
  // key_cfg.mode = GPIO_MODE_INPUT;
  // key_cfg.intr_type = GPIO_INTR_NEGEDGE;
  // gpio_config(&key_cfg);
}