#pragma once

#include "I2cSensorBase.h"

class MpuSensor : public I2cSensorBase {
   public:
    explicit MpuSensor(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID);

    void read() override;
};