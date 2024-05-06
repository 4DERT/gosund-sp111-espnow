#include "relay.h"

#include <stdbool.h>

#include "esp-now-communication.h"
#include "pinout.h"

bool relay_get_state() {
  return (gpio_get_level(PINOUT_RELAY_GPIO) == PINOUT_RELAY_HIGH);
}

void relay_on() {
  gpio_set_level(PINOUT_RELAY_GPIO, PINOUT_RELAY_HIGH);
  gpio_set_level(PINOUT_LED_RED_GPIO, PINOUT_LED_HIGH);
}

void relay_off() {
  gpio_set_level(PINOUT_RELAY_GPIO, PINOUT_RELAY_LOW);
  gpio_set_level(PINOUT_LED_RED_GPIO, PINOUT_LED_LOW);
}

void relay_toggle() {
  const bool state = relay_get_state();
  gpio_set_level(PINOUT_RELAY_GPIO, !state);
  gpio_set_level(PINOUT_LED_RED_GPIO, (!state ? PINOUT_LED_HIGH : PINOUT_LED_LOW));
}
