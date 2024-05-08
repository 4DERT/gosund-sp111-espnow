#ifndef RELAY_H_
#define RELAY_H_

#include <stdbool.h>
#include "driver/gpio.h"

#define RELAY_GPIO GPIO_NUM_15
#define RELAY_HIGH 1
#define RELAY_LOW 0

void relay_init();
bool relay_get_state();
void relay_on();
void relay_off();
void relay_toggle();

#endif  // RELAY_H_