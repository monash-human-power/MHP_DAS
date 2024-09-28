#include <stdint.h>

#include "I2cSensorBase.h"
#include "SensorBase.h"

#ifndef MPU_SENSOR
#define MPU_SENSOR

class MpuSensor : public I2cSensorBase {
   public:
    // use parent (I2cSensorBase) constructor
    using I2cSensorBase::I2cSensorBase;

    // method
    void configure() override;
    void read() override;
};

#endif