#include <stdint.h>

#include <vector>

#include "I2cSensorBase.h"
#include "SensorBase.h"

#ifndef BAROMETER_SENSOR
#define BAROMETER_SENSOR

class BarometerSensor : public I2cSensorBase {
   public:
    const int scaleFactor = 524288;
    int16_t c0;
    int16_t c1;

    // Use parent (I2cSensorBase) constructor
    using I2cSensorBase::I2cSensorBase;

    // Methods
    void configure() override;
    void read() override;
};

#endif