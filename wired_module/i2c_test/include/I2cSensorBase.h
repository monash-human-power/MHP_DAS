#include "driver/i2c.h"

#ifndef I2C_BASE
#define I2C_BASE
class I2cSensorBase{
    protected:

    public:
        // field
        uint8_t sensorAddress;
        i2c_port_t masterPortNum;
        uint8_t readBuffer[8]; 

        // construct
        explicit I2cSensorBase(i2c_port_t masterPortNum, uint8_t sensorAddress);
        void read_sensor_register(uint8_t registerAddress,size_t readLength,int timeout);
        void write_sensor_register(uint8_t registerAddress, uint8_t data,int timeout);
};

#endif 