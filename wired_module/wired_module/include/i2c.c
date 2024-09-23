#include "driver/i2c.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2c.h"

#define I2C_NUM I2C_NUM_0
#define I2C_SCL GPIO_NUM_22
#define I2C_SDA GPIO_NUM_21
#define MPU_ADDR 0x68
#define SPL_ADDR 0x76
#define REG 0x75

static esp_err_t i2c_master_init(void) {
    i2c_port_t i2c_master_port = I2C_NUM;

    i2c_config_t conf;
    conf.sda_io_num = I2C_SDA;
    conf.mode = I2C_MODE_MASTER;
    conf.scl_io_num = I2C_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;

    i2c_param_config(i2c_master_port, &conf);
    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}

esp_err_t read_reg(uint8_t dev_address, uint8_t reg, uint8_t* readBuffer, size_t len) {
    return (i2c_master_write_read_device(I2C_NUM, dev_address, &reg, 1, readBuffer, len, 2000));
}

esp_err_t write_reg(uint8_t dev_address, uint8_t reg, uint8_t data) {
    uint8_t writeBuf[2];  // writeBuf[len+1];
    writeBuf[0] = reg;
    writeBuf[1] = data;
    return (i2c_master_write_to_device(I2C_NUM, dev_address, writeBuf, 2, 1000));
}

void binprintf(uint8_t v) {
    unsigned int mask = 1 << ((sizeof(uint8_t) << 3) - 1);
    while (mask) {
        printf("%d", (v & mask ? 1 : 0));
        mask >>= 1;
    }
}

int scaleFactor;
int16_t c0, c1;

void setup() {
    i2c_master_init();

    write_reg(MPU_ADDR, 0x6B, 0);
    write_reg(MPU_ADDR, 0x19, 7);

    uint8_t spl_data[10];

    write_reg(SPL_ADDR, 0x08, 7);  // configure the gyro to start

    read_reg(SPL_ADDR, 0x08, spl_data, 1);

    printf("spl state:");
    binprintf(spl_data[0]);  // check the state of the spl sensor

    read_reg(SPL_ADDR, 0x06, spl_data, 1);

    printf("preasure cfg:");
    binprintf(spl_data[0]);  // check the pressure sensor configuration

    read_reg(SPL_ADDR, 0x07, spl_data, 1);
    write_reg(SPL_ADDR, 0x07, 240);  // set to external sensor mode and 128 measure /sec
    printf("temp cfg:");
    binprintf(spl_data[0]);  // check the temperature sensor configuration

    read_reg(SPL_ADDR, 0x10, spl_data, 3);

    // in datasheet, depend on the oversample rate
    scaleFactor = 524288;

    // get temperature sensor constant
    c0 = spl_data[0] << 4 | spl_data[1] >> 4;
    if (c0 & (1 << 11))    // Check for 2's complement negative number
        c0 = c0 | 0XF000;  // Set left bits to one for 2's complement conversion of negative number

    c1 = ((spl_data[1] & 0xF) << 8) | spl_data[2];

    if (c1 & (1 << 11))    // Check for 2's complement negative number
        c1 = c1 | 0XF000;  // Set left bits to one for 2's complement conversion of negative number
    printf("c0:%d, c1:%d\n", c0, c1);
}

void read(uint8_t* arr, int size) {
    uint8_t mpu_data[10];
    uint8_t spl_data_msb[1];
    uint8_t spl_data_lsb[1];
    uint8_t spl_data_xlsb[1];

    read_reg(MPU_ADDR, 0x3B, mpu_data, 6);
    int16_t RAWX = (mpu_data[0] << 8) | mpu_data[1];
    int16_t RAWY = (mpu_data[2] << 8) | mpu_data[3];
    int16_t RAWZ = (mpu_data[4] << 8) | mpu_data[5];

    float xg = (float)RAWX / 16384;
    float yg = (float)RAWY / 16384;
    float zg = (float)RAWZ / 16384;

    // printf("Gyroscope:\n");
    // printf("x=%.2f\ty=%.2f\tz=%.2f\n", xg, yg, zg);  // out

    read_reg(SPL_ADDR, 0x03, spl_data_msb, 1);
    read_reg(SPL_ADDR, 0x04, spl_data_lsb, 1);
    read_reg(SPL_ADDR, 0x05, spl_data_xlsb, 1);

    int32_t raw_temp = (spl_data_msb[0] << 8) | spl_data_lsb[0];
    raw_temp = (raw_temp << 8) | spl_data_xlsb[0];

    if (raw_temp & (1 << 23)) {
        raw_temp = raw_temp | 0XFF000000;
    }

    float temp = (float)raw_temp / (float)scaleFactor;

    temp = c0 * 0.5 + c1 * temp;  // out
    // printf("Temperature: %.3f\n", temp);
    // printf("\n");
    // vTaskDelay(75);
    arr[0] = temp;
    arr[1] = (uint8_t)(xg * 100.0f);
    arr[2] = (uint8_t)(yg * 100.0f);
    arr[3] = (uint8_t)(zg * 100.0f);
}