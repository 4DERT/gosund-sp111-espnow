#ifndef PROTOCOL_MESSAGES_H_
#define PROTOCOL_MESSAGES_H_

#define PM_INFO "{\"data\":[0],\"cmd\":[1,3]}"
#define PM_PAIR_REQUEST "SHPR:" PM_INFO
#define PM_PAIR_ACCEPT "SHPR:PAIRED"

#define PM_CMD_PARAMETER_SIZE 8
#define PM_CMD_PARAMETER_COUNT 2
#define PM_CMD_FMT "C:%d,%8s"

#define PM_CMD_1 1
#define PM_CMD_1_ALL "ALL"
#define PM_CMD_1_3 "3"

#define PM_CMD_3 3
#define PM_CMD_3_ON "ON"
#define PM_CMD_3_OFF "OFF"
#define PM_CMD_3_TOGGLE "TOGGLE"

#define PM_STATUS_3_SIZE 8
#define PM_STATUS_3_FMT "S:3,%s"

#endif  // PROTOCOL_MESSAGES_H_