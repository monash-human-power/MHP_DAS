// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <CAN.h>
#include <MpuSensor.h>

#include "driver/i2c.h"

#define I2C_NUM I2C_NUM_0
#define I2C_SCL GPIO_NUM_22
#define I2C_SDA GPIO_NUM_21

// Setup MPU sensor
MpuSensor mpuSensor(I2C_NUM, 0x68, 0x13);

static esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000,
        }

    };
    i2c_param_config(I2C_NUM, &conf);
    return i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);
}

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
    // Setup CAN bus
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

    // Initialize I2C
    i2c_master_init();

    mpuSensor.configure();
}

void loop() {
    mpuSensor.send();
    delay(1000);
}