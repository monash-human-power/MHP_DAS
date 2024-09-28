#include "driver/i2c_master.h"

#ifndef I2C_BASE
#define I2C_BASE
class I2cSensorBase{
    protected:

    public:
        // field
        i2c_device_config_t sensorConfig;
        i2c_master_dev_handle_t sensorHandle;
        uint8_t *readBuffer;

        // construc
        explicit I2cSensorBase(i2c_addr_bit_len_t addressLength, uint8_t sensorAddress,int clockFrequency);
        void read_sensor_register(uint8_t registerAddress,size_t readLength,int timeout);
        void write_sensor_register(uint8_t registerAddress, uint8_t data,int timeout);
        void link_device_to_master(i2c_master_bus_handle_t busHandle);
};

#endif 