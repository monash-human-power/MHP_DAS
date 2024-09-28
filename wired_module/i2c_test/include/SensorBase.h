#include <stdint.h>

#ifndef SENSOR_BASE
#define SENSOR_BASE

// abstract class
class SensorBase {
    
    public:
        uint8_t canBuffer[4]; 
        
        // abstract method
        virtual void configure() = 0;
        virtual void read() = 0;
        
        void send(){


        }
};

#endif