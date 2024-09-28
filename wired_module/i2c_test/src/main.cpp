#define I2C_NUM I2C_NUM_0
#define I2C_SCL GPIO_NUM_22
#define I2C_SDA GPIO_NUM_21

#include "MpuSensor.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

// enables use of c++
#ifdef __cplusplus
extern "C" {
    void app_main(void);
}

static esp_err_t i2c_master_init(void){

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_SDA,
        .scl_io_num = I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master = {
            .clk_speed = 400000,
        }

    };
    i2c_param_config(I2C_NUM, &conf);
    return i2c_driver_install(I2C_NUM, conf.mode, 0, 0, 0);
}

void app_main(){

    i2c_master_init();

	MpuSensor mpuSensor(I2C_NUM,0x68);
        
    mpuSensor.configure();

    while (1){
        std::vector<float> data = mpuSensor.read();
        printf("x=%.2f\ty=%.2f\tz=%.2f\n", data[0], data[1], data[2]);
        vTaskDelay(75);
    }

}
#endif
