#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// sdkconfig
#include "../build/include/sdkconfig.h"

// FreeRTOS
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "portmacro.h"

// ESP
#include "driver/gpio.h"
#include "driver/uart.h"
#include "errno.h"
#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"

// project includes
#include "pinout.h"
#include "esp-now-communication.h"
#include "pair.h"

static void button_isr_handler(void* arg) {}

void app_main() {
  uart_set_baudrate(0, 115200);
  pinout_gpio_init(button_isr_handler);
  nvs_flash_init();
  esp_now_communication_init();

  bool is_pressed = (gpio_get_level(PINOUT_BUTTON_GPIO) == PINOUT_BUTTON_PRESSED);
  init_gateway_mac(is_pressed);

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
