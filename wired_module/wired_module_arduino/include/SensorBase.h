#include <CAN.h>
#include <stdint.h>

#ifndef SENSOR_BASE
#define SENSOR_BASE

// abstract class
class SensorBase {
   public:
    uint8_t canBuffer[4];
    uint8_t sensorID;

    // abstract method
    virtual void configure() = 0;
    virtual void read() = 0;

    void send() {
        CAN.beginPacket(this->sensorID);                      // module | sensor // 0001 | 0001
        this->read();                                         // Read data from sensor and store in buffer
        CAN.write(this->canBuffer, sizeof(this->canBuffer));  // Send packet
        CAN.endPacket();
    }
};

#endif