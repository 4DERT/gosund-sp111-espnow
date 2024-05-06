#ifndef MESSAGE_HANDLER_H_
#define MESSAGE_HANDLER_H_

#include "esp-now-communication.h"

void msgh_send_cmd_3_status();
void msgh_parse_gateway_message(espnow_event_receive_cb_t* data);

#endif //MESSAGE_HANDLER_H_