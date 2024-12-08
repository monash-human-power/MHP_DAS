#pragma once

#include "driver/i2c.h"

class I2cMaster {
   public:
    i2c_port_t portNum;

    I2cMaster(i2c_port_t portNum, int sda, int scl, int clockFrequency);
    ~I2cMaster();
};