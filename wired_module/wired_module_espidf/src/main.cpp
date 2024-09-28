/*
 * SPDX-FileCopyrightText: 2010-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

/*
 * The following example demonstrates the self testing capabilities of the TWAI
 * peripheral by utilizing the No Acknowledgment Mode and Self Reception Request
 * capabilities. This example can be used to verify that the TWAI peripheral and
 * its connections to the external transceiver operates without issue. The example
 * will execute multiple iterations, each iteration will do the following:
 * 1) Start the TWAI driver
 * 2) Transmit and receive 100 messages using self reception request
 * 3) Stop the TWAI driver
 */

#include <stdio.h>
#include <stdlib.h>

#include "driver/twai.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "i2c.cpp"

/* --------------------- Definitions and static variables ------------------ */

// Example Configurations
#define NO_OF_MSGS 100
#define NO_OF_ITERS 3
#define TX_GPIO_NUM (gpio_num_t)17
#define RX_GPIO_NUM (gpio_num_t)16
#define TX_TASK_PRIO 8    // Sending task priority
#define RX_TASK_PRIO 9    // Receiving task priority
#define CTRL_TSK_PRIO 10  // Control task priority
#define EXAMPLE_TAG "Self Test"

// Specify the file to be cpp
extern "C" {
void app_main(void);
}

static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_25KBITS();
// Filter all other IDs except MSG_ID
static const twai_filter_config_t f_config = {.acceptance_code = 0, .acceptance_mask = 0xFFFFFFFF, .single_filter = true};

// Set to NO_ACK mode due to self testing with single module
static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NO_ACK);

static SemaphoreHandle_t tx_sem;
static SemaphoreHandle_t rx_sem;
static SemaphoreHandle_t ctrl_sem;
static SemaphoreHandle_t done_sem;

/* --------------------------- Tasks and Functions -------------------------- */

static void twai_transmit_task(void *arg) {
    while (1) {
        twai_message_t tx_msg_temp = {
            // Message type and format settings
            .extd = 0,          // Standard Format message (11-bit ID)
            .rtr = 0,           // Send a data frame
            .ss = 0,            // Not single shot
            .self = 1,          // Message is a self reception request (loopback)
            .dlc_non_comp = 0,  // DLC is less than 8
            // Message ID and payload
            .identifier = (uint32_t)0x555,
            .data_length_code = 8,
        };

        twai_message_t tx_msg_gyro_x = {
            // Message type and format settings
            .extd = 0,          // Standard Format message (11-bit ID)
            .rtr = 0,           // Send a data frame
            .ss = 0,            // Not single shot
            .self = 1,          // Message is a self reception request (loopback)
            .dlc_non_comp = 0,  // DLC is less than 8
            // Message ID and payload
            .identifier = (uint32_t)0x556,
            .data_length_code = 8,
        };

        xSemaphoreTake(tx_sem, portMAX_DELAY);
        for (int i = 0; i < NO_OF_MSGS; i++) {
            // Transmit messages using self reception request

            // read data from sensor and move into can bus message
            readTemp(tx_msg_temp.data);
            readGyroX(tx_msg_gyro_x.data);

            // send
            ESP_ERROR_CHECK(twai_transmit(&tx_msg_temp, portMAX_DELAY));
            ESP_ERROR_CHECK(twai_transmit(&tx_msg_gyro_x, portMAX_DELAY));

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    vTaskDelete(NULL);
}

static void twai_receive_task(void *arg) {
    twai_message_t rx_message;

    while (1) {
        xSemaphoreTake(rx_sem, portMAX_DELAY);

        for (int i = 0; i < NO_OF_MSGS; i++) {
            // Receive message and print message data
            ESP_ERROR_CHECK(twai_receive(&rx_message, portMAX_DELAY));
            ESP_LOGI(EXAMPLE_TAG, "Msg received\tID 0x%lx\tData = %d, %d, %d, %d, %d, %d, %d, %d",
                     rx_message.identifier,
                     rx_message.data[0], rx_message.data[1], rx_message.data[2], rx_message.data[3],
                     rx_message.data[4], rx_message.data[5], rx_message.data[6], rx_message.data[7]);

            // Convert binary data back to float
            ESP_LOGI(EXAMPLE_TAG, "Msg received\tID 0x%lx\tConverted Data: %f", rx_message.identifier, *((float *)rx_message.data));
        }

        // Indicate to control task all messages received for this iteration
        xSemaphoreGive(ctrl_sem);
    }
    vTaskDelete(NULL);
}

static void twai_control_task(void *arg) {
    xSemaphoreTake(ctrl_sem, portMAX_DELAY);
    while (1) {
        // Start TWAI Driver for this iteration
        ESP_ERROR_CHECK(twai_start());
        ESP_LOGI(EXAMPLE_TAG, "Driver started");

        // Setup I2C
        setup();

        // Trigger TX and RX tasks to start transmitting/receiving
        xSemaphoreGive(rx_sem);
        xSemaphoreGive(tx_sem);
        xSemaphoreTake(ctrl_sem, portMAX_DELAY);  // Wait for TX and RX tasks to finish iteration

        ESP_ERROR_CHECK(twai_stop());  // Stop the TWAI Driver
        ESP_LOGI(EXAMPLE_TAG, "Driver stopped");
        vTaskDelay(pdMS_TO_TICKS(100));  // Delay then start next iteration
    }
    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
}

void app_main(void) {
    // Create tasks and synchronization primitives
    tx_sem = xSemaphoreCreateBinary();
    rx_sem = xSemaphoreCreateBinary();
    ctrl_sem = xSemaphoreCreateBinary();
    done_sem = xSemaphoreCreateBinary();

    xTaskCreatePinnedToCore(twai_control_task, "TWAI_ctrl", 4096, NULL, CTRL_TSK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(twai_receive_task, "TWAI_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(twai_transmit_task, "TWAI_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);

    // Install TWAI driver
    ESP_ERROR_CHECK(twai_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");

    // Start control task
    xSemaphoreGive(ctrl_sem);
    // Wait for all iterations and tasks to complete running
    xSemaphoreTake(done_sem, portMAX_DELAY);

    // Uninstall TWAI driver
    ESP_ERROR_CHECK(twai_driver_uninstall());
    ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");

    // Cleanup
    vSemaphoreDelete(tx_sem);
    vSemaphoreDelete(rx_sem);
    vSemaphoreDelete(ctrl_sem);
    vQueueDelete(done_sem);
}