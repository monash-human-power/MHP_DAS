#pragma once

#include "I2cSensorBase.h"

class BarometerSensor : public I2cSensorBase {
   public:
    BarometerSensor(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID);

    void read() override;

   private:
    int16_t c0;
    int16_t c1;
};