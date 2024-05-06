#include "pair.h"

#include <string.h>

#include "esp-now-communication.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "protocol_messages.h"

#define NVS_MAC_KEY "gw_mac"
#define NVS_NAME "PAIR"

static const char* TAG = "pair";

static mac_t gateway;
static bool is_paired = false;
static bool is_pairing = false;

static nvs_handle_t nvs;
static TaskHandle_t pair_task_handle;

const mac_t* get_gateway_mac() {
  if (!is_paired) return NULL;
  return &gateway;
}

void pair_task(void* params) {
  esp_now_send_t data = {
      .data = PM_PAIR_REQUEST,
      .dest_mac.value = esp_now_broadcast_mac.value,  // broadcast mac
      .ack_queue = NULL};

  while (1) {
    ESP_LOGI(TAG, "Sending pair request");
    xQueueSend(esp_now_send_queue, &data, 0);

    // wait 5 second for response
    int time = 4000 + (esp_random() % 2000);
    if (ulTaskNotifyTake(true, pdMS_TO_TICKS(time))) {
      ESP_LOGI(TAG, "Device " MACSTR " accepted pair", MAC2STR(gateway.bytes));
      is_pairing = false;
      is_paired = true;
      nvs_set_u64(nvs, NVS_MAC_KEY, gateway.value);
      nvs_commit(nvs);
      pair_task_handle = NULL;
      vTaskDelete(NULL);
    }
  }
}

void check_received_pairing_acceptance(espnow_event_receive_cb_t* data) {
  if (!strcmp(data->data, PM_PAIR_ACCEPT) && pair_task_handle != NULL && is_pairing) {
    gateway.value = data->src_mac.value;
    xTaskNotifyGive(pair_task_handle);
  }
}

void init_gateway_mac(bool force_pair) {
  is_pairing = true;
  is_paired = false;

  nvs_open(NVS_NAME, NVS_READWRITE, &nvs);

  if (force_pair) {
    ESP_LOGI(TAG, "force_pair");
    nvs_set_u64(nvs, NVS_MAC_KEY, 0ULL);
    nvs_commit(nvs);
  } else {
    nvs_get_u64(nvs, NVS_MAC_KEY, &gateway.value);
    ESP_LOGI(TAG, "MAC from NVS - " MACSTR, MAC2STR(gateway.bytes));

    if (gateway.value) {
      ESP_LOGI(TAG, "Got gateway MAC from NVS - " MACSTR, MAC2STR(gateway.bytes));
      is_pairing = false;
      is_paired = true;
    }
  }

  if (!is_paired) {
    ESP_LOGI(TAG, "Starting the pairing procedure");
    xTaskCreate(pair_task, "pair_task", 2048, NULL, 1, &pair_task_handle);
  }
}