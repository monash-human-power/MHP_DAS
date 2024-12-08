#pragma once

#include "constants.h"

#include <CAN.h>

class SensorBase {
   public:
    explicit SensorBase(uint8_t sensorID);

    virtual void read() = 0;
    void send();

   protected:
    uint8_t canBuffer[CANConfig::PACKET_SIZE];

   private:
    uint8_t sensorID;
};