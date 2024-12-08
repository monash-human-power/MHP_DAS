#include "I2cSensorBase.h"

I2cSensorBase::I2cSensorBase(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID)
    : SensorBase(sensorID), sensorAddress(sensorAddress), masterPortNum(masterPortNum) {}

void I2cSensorBase::read_sensor_register(uint8_t registerAddress, size_t readLength, int timeout) {
    i2c_master_write_read_device(this->masterPortNum, this->sensorAddress, &registerAddress,
                                 1, this->readBuffer, readLength, timeout);
}

void I2cSensorBase::write_sensor_register(uint8_t registerAddress, uint8_t data, int timeout) {
    uint8_t writeBuf[2];
    writeBuf[0] = registerAddress;
    writeBuf[1] = data;
    i2c_master_write_to_device(this->masterPortNum, this->sensorAddress, writeBuf, 2, timeout);
}