#ifndef BUTTON_H_
#define BUTTON_H_

#include <stdbool.h>
#include "driver/gpio.h"

#define BUTTON_BOUNCE_TIME_MS 300
#define BUTTON_GPIO GPIO_NUM_13
#define BUTTON_PRESSED_LEVEL 0
#define BUTTON_TASK_STACK_DEPTH 2048
#define BUTTON_TASK_PRIORITY 1

void button_init();
bool button_is_pressed();

#endif //BUTTON_H_