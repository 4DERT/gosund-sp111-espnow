#include "message_handler.h"

#include <string.h>
#include "esp_log.h"

#include "esp-now-communication.h"
#include "pair.h"
#include "protocol_messages.h"
#include "relay.h"

static const char* TAG = "msgh";

static void parse_cmd_1(const char* parameter);
static void parse_cmd_3(const char* parameter);

void msgh_parse_gateway_message(espnow_event_receive_cb_t* data) {
  const mac_t* gateway_mac = get_gateway_mac();

  if (gateway_mac == NULL || data->src_mac.value != gateway_mac->value)
    return;

  int command_number = -1;
  char parameter[PM_CMD_PARAMETER_SIZE] = {'\0'};

  int parsed = sscanf(data->data, PM_CMD_FMT, &command_number, parameter);

  if (parsed != PM_CMD_PARAMETER_COUNT) {
    ESP_LOGW(TAG, "Invalid format");
    return;
  }

  switch (command_number) {
    case PM_CMD_1:
      parse_cmd_1(parameter);
      break;

    case PM_CMD_3:
      parse_cmd_3(parameter);
      break;

    default:
      ESP_LOGW(TAG, "Invalid command number");
      break;
  }
}

static void parse_cmd_1(const char* parameter) {
  if (!strcmp(PM_CMD_1_3, parameter) ||
      !strcmp(PM_CMD_1_ALL, parameter)) {
    msgh_send_cmd_3_status();
  } else {
    ESP_LOGW(TAG, "Invalid command 1 parameter");
  }
}

static void parse_cmd_3(const char* parameter) {
  if (!strcmp(PM_CMD_3_ON, parameter)) {
    relay_on();
    msgh_send_cmd_3_status();
  } else if (!strcmp(PM_CMD_3_OFF, parameter)) {
    relay_off();
    msgh_send_cmd_3_status();
  } else if (!strcmp(PM_CMD_3_TOGGLE, parameter)) {
    relay_toggle();
    msgh_send_cmd_3_status();
  } else {
    ESP_LOGW(TAG, "Invalid command 3 parameter");
  }
}

void msgh_send_cmd_3_status() {
  const mac_t* gateway_mac = get_gateway_mac();

  if(gateway_mac == NULL) {
    ESP_LOGW(TAG, "Device is not paired");
    return;
  }

  esp_now_send_t data = {
      .dest_mac = {.value = gateway_mac->value},
      .ack_queue = NULL};

  bool status = relay_get_state();
  sprintf(data.data, PM_STATUS_3_FMT, (status ? PM_CMD_3_ON : PM_CMD_3_OFF));

  xQueueSend(esp_now_send_queue, &data, portMAX_DELAY);
}