// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>

#include "driver/i2c.h"
#include "i2c.h"

void onReceive(int packetSize) {
    // Get all bytes from packet
    uint8_t buffer[4];
    int i = 0;
    while (CAN.available()) {
        buffer[i] = CAN.read();
        i++;
    };

    // Convert bytes to float
    float data;
    memcpy(&data, buffer, sizeof(float));

    Serial.printf("Msg received | ID = 0x%lx | Data = %f\n", CAN.packetId(), data);
}

void setup() {
    // Uninstall old i2c driver
    // i2c_driver_delete(I2C_NUM_0);
    // while (1);

    // Setup i2c and CAN bus
    setup_i2c();
    CAN.setPins(16, 17);
    Serial.begin(115200);
    while (!Serial);

    // Start the CAN bus at 500 kbps
    if (!CAN.begin(500E3)) {
        Serial.println("\n\nStarting CAN failed!\n\n");
        while (1);
    }

    // Configure can bus to loopback mode for self test
    CAN.loopback();

    // Set up receive callback
    CAN.onReceive(onReceive);
}

void loop() {
    // Send temperature data
    CAN.beginPacket(0x11);  // module | sensor // 0001 | 0001
    uint8_t bufferT[4];
    readTemp(bufferT);                    // Read data from sensor and store in buffer
    CAN.write(bufferT, sizeof(bufferT));  // Send packet
    CAN.endPacket();

    // Send gyroscope data
    CAN.beginPacket(0x13);  // module | sensor // 0001 | 0011
    uint8_t bufferG[4];
    readGyroZ(bufferG);
    CAN.write(bufferG, sizeof(bufferG));
    CAN.endPacket();

    delay(1000);
}