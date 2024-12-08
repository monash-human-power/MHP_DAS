#include <BarometerSensor.h>
#include <I2cMaster.h>
#include <MpuSensor.h>
#include "utils.h"

#include <CAN.h>
#include "driver/i2c.h"

#include <vector>

// ==================================================================
// SETUP SENSORS HERE
// ==================================================================
I2cMaster i2cMaster(I2CConfig::NUM, I2CConfig::SDA, I2CConfig::SCL, I2CConfig::FREQ_HZ);

MpuSensor mpuSensor(i2cMaster.portNum, MPUConfig::ADDR, MPUConfig::ID);
BarometerSensor barometerSensor(i2cMaster.portNum, BarometerConfig::ADDR, BarometerConfig::ID);

std::vector<SensorBase *> sensors = {&mpuSensor, &barometerSensor};
// ==================================================================

void setup() {
    // Setup CAN bus
    CAN.setPins(CANConfig::RX_PIN, CANConfig::TX_PIN);
    Serial.begin(115200);
    while (!Serial);

    // Start CAN bus
    if (!CAN.begin(CANConfig::BAUD_RATE)) {
        Serial.println("\n\nStarting CAN failed!\n\n");
        while (1);
    }

    // DELETE WHEN CONNECTED TO RASP PI (used for self testing)
    CAN.loopback();
    CAN.onReceive(onReceive);
}

void loop() {
    for (auto sensor : sensors) {
        sensor->send();
    }
    delay(1000);
}