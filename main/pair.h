#ifndef PAIR_H_
#define PAIR_H_

#include "esp-now-communication.h"

void init_gateway_mac(bool force_pair);
void check_received_pairing_acceptance(espnow_event_receive_cb_t* data);

#endif //PAIR_H_