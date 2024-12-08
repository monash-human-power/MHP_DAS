#pragma once

#include "SensorBase.h"
#include "constants.h"

#include <driver/i2c.h>

class I2cSensorBase : public SensorBase {
   public:
    explicit I2cSensorBase(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID);

    void read_sensor_register(uint8_t registerAddress, size_t readLength, int timeout);
    void write_sensor_register(uint8_t registerAddress, uint8_t data, int timeout);

   protected:
    uint8_t readBuffer[I2CConfig::READ_BUFFER_SIZE];

   private:
    uint8_t sensorAddress;
    i2c_port_t masterPortNum;
};