#include <stdint.h>
#include <vector>
#include "I2cSensorBase.h"
#include "SensorBase.h"
#include "driver/i2c_master.h"

#ifndef MPU_SENSOR
#define MPU_SENSOR

class MpuSensor: public I2cSensorBase, public SensorBase{
    public:
        // use parent (I2cSensorBase) constructor
        using I2cSensorBase::I2cSensorBase;
        
        // destructor
        ~MpuSensor();
        
        // method
        void configure() override;
        void read() override;
};

#endif