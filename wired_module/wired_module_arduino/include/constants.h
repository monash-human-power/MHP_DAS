#pragma once

#include <driver/i2c.h>

namespace CANConfig {
    const int RX_PIN = 16;
    const int TX_PIN = 17;
    const double BAUD_RATE = 500E3;
    const int PACKET_SIZE = 4;
}

namespace I2CConfig {
    const i2c_port_t NUM = I2C_NUM_0;
    const int SCL = GPIO_NUM_22;
    const int SDA = GPIO_NUM_21;
    const int FREQ_HZ = 400000;
    const int READ_BUFFER_SIZE = 8;
}

namespace MPUConfig {
    const int ID = 0x13;
    const int ADDR = 0x68;
}

namespace BarometerConfig {
    const int ID = 0x11;
    const int ADDR = 0x76;
    const int SCALE_FACTOR = 524288;
}
