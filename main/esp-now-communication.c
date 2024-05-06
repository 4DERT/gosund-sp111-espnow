#include "esp-now-communication.h"

#include <string.h>

#include "FreeRTOS.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "pair.h"
#include "pinout.h"
#include "message_handler.h"

static const char *TAG = "esp now com";

#define ESPNOW_MAXDELAY 100

#define IS_BROADCAST_ADDR(addr) (memcmp(addr, esp_now_broadcast_mac.bytes, ESP_NOW_ETH_ALEN) == 0)
const mac_t esp_now_broadcast_mac = {.bytes = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};

static void on_esp_now_data_send(const uint8_t *mac_addr, esp_now_send_status_t status);
static void on_esp_now_data_receive(const uint8_t *mac_addr, const uint8_t *data, int data_len);
static void esp_now_send_task(void *params);
static void esp_now_receive_task(void *params);

typedef struct {
  uint8_t mac_addr[ESP_NOW_ETH_ALEN];
  esp_now_send_status_t status;
} espnow_event_send_cb_t;

QueueHandle_t esp_now_send_queue;
static QueueHandle_t receive_queue;
static QueueHandle_t send_result_queue;

void esp_now_communication_init() {
  // init wifi module
  esp_netif_init();
  esp_event_loop_create_default();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_start();
  esp_wifi_set_channel(ESP_NOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // Print MAC
  uint8_t mac[ESP_NOW_ETH_ALEN];
  esp_wifi_get_mac(ESP_IF_WIFI_STA, mac);
  printf("ESP_NOW_MAC: " MACSTR "\n", MAC2STR(mac));

  // init queue
  esp_now_send_queue = xQueueCreate(ESP_NOW_SEND_QUEUE_SIZE, sizeof(esp_now_send_t));
  receive_queue = xQueueCreate(ESP_NOW_RECIEVE_QUEUE_SIZE, sizeof(espnow_event_receive_cb_t));
  send_result_queue = xQueueCreate(ESP_NOW_RESULT_QUEUE_SIZE, sizeof(espnow_event_send_cb_t));

  // esp now init
  if (esp_now_init() != ESP_OK) {
    ESP_LOGE(TAG, "esp_now_init ERROR");
  }

  // register esp now callbacks
  esp_now_register_send_cb(on_esp_now_data_send);
  esp_now_register_recv_cb(on_esp_now_data_receive);

  // create send task
  xTaskCreate(esp_now_send_task, "esp_now_send_task", 4096, NULL, 5, NULL);
  xTaskCreate(esp_now_receive_task, "esp_now_receive_task", 4096, NULL, 5, NULL);
}

static void on_esp_now_data_send(const uint8_t *mac_addr, esp_now_send_status_t status) {
  espnow_event_send_cb_t send_cb;
  memcpy(send_cb.mac_addr, mac_addr, ESP_NOW_ETH_ALEN);
  send_cb.status = status;

  if (xQueueSend(send_result_queue, &send_cb, ESPNOW_MAXDELAY) != pdTRUE) {
    ESP_LOGW(TAG, "Send send queue fail");
  }
}

static void on_esp_now_data_receive(const uint8_t *src_addr, const uint8_t *data, int data_len) {
  if (src_addr == NULL || data == NULL || data_len <= 0) {
    ESP_LOGE(TAG, "Receive cb arg error");
    return;
  }

  espnow_event_receive_cb_t cb;
  memset(&cb, 0, sizeof(espnow_event_receive_cb_t));
  memcpy(&cb.src_mac, src_addr, ESP_NOW_ETH_ALEN);
  memcpy(&cb.data, data, data_len);
  cb.data_len = data_len;

  if (xQueueSend(receive_queue, &cb, ESPNOW_MAXDELAY) != pdTRUE) {
    ESP_LOGW(TAG, "Send receive queue fail");
  }
}

QueueHandle_t esp_now_create_send_ack_queue() {
  return xQueueCreate(2, sizeof(esp_now_send_status_t));
}

void esp_now_send_task(void *params) {
  static const char *TAG = "espnow_send_task";
  BaseType_t queue_status;
  esp_now_send_t data;
  esp_now_peer_info_t peer_info;
  esp_err_t err = ESP_OK;
  espnow_event_send_cb_t result;

  while (1) {
    // wait for data
    queue_status = xQueueReceive(esp_now_send_queue, &data, portMAX_DELAY);
    if (queue_status != pdPASS)
      continue;

    gpio_set_level(PINOUT_LED_BLUE_GPIO, PINOUT_LED_HIGH);

    // create peer
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t));
    peer_info.channel = ESP_NOW_CHANNEL;
    peer_info.encrypt = false;
    memcpy(peer_info.peer_addr, data.dest_mac.bytes, 6);
    err = esp_now_add_peer(&peer_info);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Error while adding new peer!");
      continue;
    }

    // send data
    err = esp_now_send(peer_info.peer_addr, (uint8_t *)&(data.data), strlen(data.data));
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Error while sending!");
      continue;
    }

    // wait for the data to be sent
    queue_status = xQueueReceive(send_result_queue, &result, portMAX_DELAY);
    if (result.status == ESP_NOW_SEND_FAIL) {
      ESP_LOGW(TAG, "Sent data to " MACSTR ", with status %d (not received)",
               MAC2STR(result.mac_addr), result.status);
    } else {
      ESP_LOGI(TAG, "Sent data to " MACSTR ", with status %d (received)",
               MAC2STR(result.mac_addr), result.status);
    }

    if (data.ack_queue != NULL)
      xQueueSend(*data.ack_queue, &result.status, 0);

    // remove peer
    esp_now_del_peer(peer_info.peer_addr);

    gpio_set_level(PINOUT_LED_BLUE_GPIO, PINOUT_LED_LOW);
  }
}

void esp_now_receive_task(void *params) {
  static const char *TAG = "espnow_receive_task";
  BaseType_t queue_status;
  espnow_event_receive_cb_t data;

  while (1) {
    // wait for data
    queue_status = xQueueReceive(receive_queue, &data, portMAX_DELAY);
    if (queue_status != pdPASS)
      continue;

    // Parsing data
    ESP_LOGI(TAG, "Recieved message \"%s\" from " MACSTR, data.data, MAC2STR(data.src_mac.bytes));

    if (IS_BROADCAST_ADDR(data.src_mac.bytes)) {
      ESP_LOGI(TAG, "Receive broadcast ESPNOW data");
    }

    check_received_pairing_acceptance(&data);
    msgh_parse_gateway_message(&data);
  }
}