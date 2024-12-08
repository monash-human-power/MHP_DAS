#include "utils.h"

#include <CAN.h>

#include <cstring>

// Callback function for CAN bus. Should be configured and called by rasp pi after removing loopback mode
void onReceive(int packetSize) {
    uint8_t buffer[4];
    float data;

    // Get all bytes from packet
    int i = 0;
    while (CAN.available()) {
        buffer[i] = CAN.read();
        i++;
    };

    // Convert bytes to float
    memcpy(&data, buffer, sizeof(float));

    Serial.printf("Msg received | ID = 0x%lx | Data = %f\n", CAN.packetId(), data);
}