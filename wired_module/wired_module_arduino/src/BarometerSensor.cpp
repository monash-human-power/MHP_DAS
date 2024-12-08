#include "BarometerSensor.h"
#include "constants.h"

#include <cstring>

// TODO: Add comments and remove magic numbers

BarometerSensor::BarometerSensor(i2c_port_t masterPortNum, uint8_t sensorAddress, uint8_t sensorID)
    : I2cSensorBase(masterPortNum, sensorAddress, sensorID) {
    read_sensor_register(0x10, 3, 1000);
    
    this->c0 = this->readBuffer[0] << 4 | this->readBuffer[1] >> 4;
    if (this->c0 & (1 << 11))
        this->c0 = this->c0 | 0XF000;

    this->c1 = ((this->readBuffer[1] & 0xF) << 8) | this->readBuffer[2];

    if (this->c1 & (1 << 11))
        this->c1 = this->c1 | 0XF000;
}

void BarometerSensor::read() {
    read_sensor_register(0x03, 3, 1000);

    int32_t raw_temp = (this->readBuffer[0] << 8) | this->readBuffer[1];
    raw_temp = (raw_temp << 8) | this->readBuffer[2];

    if (raw_temp & (1 << 23)) {raw_temp = raw_temp | 0XFF000000;}

    float temp = (float)raw_temp / (float)BarometerConfig::SCALE_FACTOR;
    temp = this->c0 * 0.5 + this->c1 * temp;

    memcpy(this->canBuffer, &temp, sizeof(temp));
}
