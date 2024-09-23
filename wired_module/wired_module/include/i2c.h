// i2c.h
#ifndef I2C_H
#define I2C_H

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void setup();

static esp_err_t i2c_master_init(void);

esp_err_t read_reg(uint8_t dev_address, uint8_t reg, uint8_t *readBuffer, size_t len);

esp_err_t write_reg(uint8_t dev_address, uint8_t reg, uint8_t data);

void binprintf(uint8_t v);

void read(uint8_t *, int);

#endif