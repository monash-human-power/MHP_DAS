#include "MpuSensor.h"
#include <vector>

void MpuSensor::configure(){
   write_sensor_register(0x6B,0,2000);
   write_sensor_register(0x19,7,2000);
}

std::vector<float> MpuSensor::read(){
    std::vector<float> data;
    read_sensor_register(0x3B,6,2000);

    int16_t RAWX = (this -> readBuffer[0]<<8) | this -> readBuffer[1];
    int16_t RAWY = (this -> readBuffer[2]<<8) | this -> readBuffer[3];
    int16_t RAWZ = (this -> readBuffer[4]<<8) | this -> readBuffer[5];
    
    float xg = (float)RAWX/16384;
    float yg = (float)RAWY/16384;
    float zg = (float)RAWZ/16384;

    data = {xg,yg,zg};
    return data;
}