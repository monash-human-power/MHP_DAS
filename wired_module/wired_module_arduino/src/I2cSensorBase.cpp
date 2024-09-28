#include "I2cSensorBase.h"

#include "driver/i2c.h"

I2cSensorBase::I2cSensorBase(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID) {
    this->masterPortNum = masterPortNum;
    this->sensorAddress = sensorAddress;
    this->sensorID = sensorID;
}

void I2cSensorBase::read_sensor_register(uint8_t registerAddress, size_t readLength, int timeout) {
    i2c_master_write_read_device(this->masterPortNum, this->sensorAddress, &registerAddress,
                                 1, this->readBuffer, readLength, timeout);
}

void I2cSensorBase::write_sensor_register(uint8_t registerAddress, uint8_t data, int timeout) {
    uint8_t writeBuf[2];  // writeBuf[len+1];
    writeBuf[0] = registerAddress;
    writeBuf[1] = data;
    i2c_master_write_to_device(this->masterPortNum, this->sensorAddress, writeBuf, 2, timeout);
}

// void I2cSensorBase::link_device_to_master(i2c_master_bus_handle_t busHandle) {
//     i2c_master_bus_add_device(busHandle,&sensorConfig,&sensorHandle);
// }
