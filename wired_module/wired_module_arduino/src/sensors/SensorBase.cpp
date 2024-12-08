#include "SensorBase.h"

SensorBase::SensorBase(uint8_t sensorID) : sensorID(sensorID) {}

void SensorBase::send() {
    CAN.beginPacket(this->sensorID);
    this->read();
    CAN.write(this->canBuffer, sizeof(this->canBuffer));
    CAN.endPacket();
}

