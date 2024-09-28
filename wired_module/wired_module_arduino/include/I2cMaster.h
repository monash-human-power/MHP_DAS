#include "driver/i2c.h"

class I2cMaster {
    public:
        // Field
        i2c_port_t portNum;

        // Constructor 
        I2cMaster(i2c_port_t portNum,int sda, int scl,int clockFrequency);
        // Destructor
        ~I2cMaster();
};