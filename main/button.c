#include "button.h"

#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "message_handler.h"
#include "relay.h"

static TaskHandle_t btn_task_handle;

#define BIT_ISR (1 << 0)
#define BIT_IGNORE (1 << 1)

void button_isr_handler(void* arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  xTaskNotifyFromISR(btn_task_handle,
                     BIT_ISR,
                     eSetBits,
                     &xHigherPriorityTaskWoken);

  if (xHigherPriorityTaskWoken == pdTRUE) {
    portYIELD_FROM_ISR();
  }
}

void button_notify_ignore() {
  xTaskNotify(btn_task_handle, BIT_IGNORE, eSetBits);
}

static void button_task(void* params) {
  uint32_t reg = 0;

  while (1) {
    xTaskNotifyWait(false, 0xFFFFFFFF, &reg, portMAX_DELAY);

    if (reg & BIT_ISR && !(reg & BIT_IGNORE)) {
      button_notify_ignore();
      relay_toggle();
      msgh_send_cmd_3_status();
    }

    vTaskDelay(pdMS_TO_TICKS(BUTTON_BOUNCE_TIME_MS));

    xTaskNotifyWait(false, 0xFFFFFFFF, &reg, 0);
    reg = 0;

  }
}

void button_init() {
  gpio_config_t key_cfg;
  key_cfg.pin_bit_mask = (1UL << BUTTON_GPIO);
  key_cfg.pull_up_en = GPIO_PULLUP_ENABLE;
  key_cfg.mode = GPIO_MODE_INPUT;
  key_cfg.intr_type = GPIO_INTR_NEGEDGE;
  gpio_config(&key_cfg);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

  xTaskCreate(button_task, "button_task", BUTTON_TASK_STACK_DEPTH, NULL, BUTTON_TASK_PRIORITY, &btn_task_handle);
}

bool button_is_pressed() {
  return gpio_get_level(BUTTON_GPIO) == BUTTON_PRESSED_LEVEL;
}