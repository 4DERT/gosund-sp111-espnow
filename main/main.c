#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// sdkconfig
#include "../build/include/sdkconfig.h"

// ESP & FreeRTOS
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "portmacro.h"
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

void app_main() {
  uart_set_baudrate(0, 115200);

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
