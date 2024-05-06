#ifndef RELAY_H_
#define RELAY_H_

#include <stdbool.h>

bool relay_get_state();
void relay_on();
void relay_off();
void relay_toggle();

#endif  // RELAY_H_