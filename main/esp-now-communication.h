#ifndef ESP_NOW_COMMUNICATION_H_
#define ESP_NOW_COMMUNICATION_H_

#include "esp_now.h"
#include "FreeRTOS.h"
#include "freertos/queue.h"

#define ESP_NOW_CHANNEL 1
#define ESP_NOW_SEND_QUEUE_SIZE 5
#define ESP_NOW_RECIEVE_QUEUE_SIZE 5
#define ESP_NOW_RESULT_QUEUE_SIZE 2
#define ESP_NOW_WAIT_FOR_DATA_TO_BE_SEND_MS 3000

/*
Usage:

esp_now_communication_init();

QueueHandle_t result = esp_now_create_send_ack_queue();

esp_now_send_t data = {
    .data = "hello",
    .dest_mac = {0xC8, 0xC9, 0xA3, 0x30, 0x4E, 0x2E},
    .ack_queue = &result
};

xQueueSend(esp_now_send_queue, &data, 100);

esp_now_send_status_t res;
xQueueReceive(result, &res, portMAX_DELAY);
ESP_LOGI(TAG, "data sent - status: %d", res);
*/

typedef union {
  uint8_t bytes[6];
  uint64_t value;
} mac_t;

typedef struct {
  char data[ESP_NOW_MAX_DATA_LEN];
  mac_t dest_mac;
  QueueHandle_t* ack_queue;
} esp_now_send_t;

typedef struct {
  mac_t src_mac;
  char data[ESP_NOW_MAX_DATA_LEN];
  int data_len;
} espnow_event_receive_cb_t;

extern QueueHandle_t esp_now_send_queue;

QueueHandle_t esp_now_create_send_ack_queue();
void esp_now_communication_init();

#endif  // ESP_NOW_COMMUNICATION_H_