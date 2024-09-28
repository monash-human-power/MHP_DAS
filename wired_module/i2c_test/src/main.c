#define I2C_NUM I2C_NUM_0
#define I2C_SCL GPIO_NUM_22
#define I2C_SDA GPIO_NUM_21

// #include "MpuSensor.h"
// #include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

// enables use of c++
// #ifdef __cplusplus
// extern "C" {
//     void app_main(void);
// }

void app_main(){
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_NUM));
    // // Initialize i2c_master_bus_config_t following the correct field order
    // i2c_master_bus_config_t i2c_mst_config = {
    //     .i2c_port = I2C_NUM_0,              // i2c_port: I2C port (using port 0)
    //     .sda_io_num = I2C_SDA,              // sda_io_num: SDA pin
    //     .scl_io_num = I2C_SCL,              // scl_io_num: SCL pin
    //     .clk_source = I2C_CLK_SRC_DEFAULT,  // clk_source: Clock source
    //     .glitch_ignore_cnt = 7,             // glitch_ignore_cnt: Glitch ignore count
    //     .intr_priority = 1,                 // intr_priority: Interrupt priority
    //     .trans_queue_depth = 20,            // trans_queue_depth: Queue depth
    //     .flags = {                          // flags: Internal pull-up enabled
    //         .enable_internal_pullup = true
    //     }
    // };

    // i2c_master_bus_handle_t busHandle;
    // ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &busHandle));

	// MpuSensor mpuSensor(I2C_ADDR_BIT_LEN_7,0x75,100000);
    
    // for(int i=0;i<8;i++){
    //     printf("%d ..\n",mpuSensor.readBuffer[i]);
    // }
    
    // mpuSensor.link_device_to_master(busHandle);
    // mpuSensor.configure();
    // mpuSensor.read_sensor_register(0x19,1,2000);
    
    // while (1){
    //     std::vector<float> data = mpuSensor.read();
    //     printf("x=%.2f\ty=%.2f\tz=%.2f\n", data[0], data[1], data[2]);
    //     vTaskDelay(75);
    // }
}
// #endif
