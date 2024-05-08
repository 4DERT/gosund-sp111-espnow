#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// sdkconfig
#include "../build/include/sdkconfig.h"

// FreeRTOS
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "portmacro.h"

// ESP
#include "driver/uart.h"
#include "nvs.h"
#include "nvs_flash.h"

// project includes
#include "esp-now-communication.h"
#include "pair.h"
#include "button.h"
#include "relay.h"
#include "leds.h"

void app_main() {
  uart_set_baudrate(0, 115200);
  nvs_flash_init();
  leds_init();
  relay_init();
  button_init();

  esp_now_communication_init();
  init_gateway_mac(button_is_pressed());
}
