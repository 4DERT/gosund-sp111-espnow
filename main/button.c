#include "button.h"

#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "message_handler.h"
#include "pinout.h"
#include "relay.h"

static TaskHandle_t btn_task_handle;

void button_isr_handler(void* arg) {
  xTaskResumeFromISR(btn_task_handle);
  taskYIELD();
}

static void button_task(void* params) {
  while (1) {
    vTaskSuspend(NULL);

    relay_toggle();
    msgh_send_cmd_3_status();

    vTaskDelay(pdMS_TO_TICKS(BUTTON_BOUNCE_TIME_MS));
  }
}

void button_init() {
  gpio_config_t key_cfg;
  key_cfg.pin_bit_mask = (1UL << PINOUT_BUTTON_GPIO);
  key_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
  key_cfg.mode = GPIO_MODE_INPUT;
  key_cfg.intr_type = GPIO_INTR_NEGEDGE;
  gpio_config(&key_cfg);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(PINOUT_BUTTON_GPIO, button_isr_handler, NULL);

  xTaskCreate(button_task, "button_task", 4096, NULL, 1, &btn_task_handle);
}
