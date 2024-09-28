#include "I2cSensorBase.h"

I2cSensorBase::I2cSensorBase(i2c_addr_bit_len_t addressLength, uint8_t sensorAddress,int clockFrequency){
    this -> sensorConfig.dev_addr_length = addressLength;
    this -> sensorConfig.device_address = sensorAddress;
    this -> sensorConfig.scl_speed_hz = clockFrequency;
    this -> readBuffer = (uint8_t*) calloc(8,sizeof(uint8_t));
}

void I2cSensorBase::read_sensor_register(uint8_t registerAddress,size_t readLength,int timeout){
   i2c_master_transmit_receive(this->sensorHandle,&registerAddress,1,this->readBuffer, readLength, timeout);
}

void I2cSensorBase::write_sensor_register(uint8_t registerAddress, uint8_t data,int timeout){
    uint8_t writeBuf[2];  // writeBuf[len+1];
	writeBuf[0] = registerAddress;
	writeBuf[1] = data;
    i2c_master_transmit(this->sensorHandle,writeBuf,2,timeout);
}

void I2cSensorBase::link_device_to_master(i2c_master_bus_handle_t busHandle) {
    i2c_master_bus_add_device(busHandle,&this->sensorConfig,&this->sensorHandle);
}

