
#include <stdio.h>
#include "driver/twai.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"


class moduleBase{
    public:
        uint16_t moduleIdentifier = 0xAAAA;//each module should have a unique identifier allows source of msg to be known



        //Initialize configuration structures using macro initializers
        twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_20, GPIO_NUM_21, TWAI_MODE_NORMAL);
        twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
        twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
        twai_message_t message;

        moduleBase(){
            // this might need to be moved to transmitMessage method if a new msg obj needs to be created each time.
            message.identifier = moduleIdentifier; //can be used to determine source of msg? and for filtering. also determines priority lower value = igher priority
            message.extd = 1; // enables 29 bit ID instead of 11
            message.data_length_code = 4; 
            main();
        }



        
    void main(){
        start();
        while(1){
            transmitMessage();
            //Wait for message to be received
            twai_message_t message;
            if (twai_receive(&message, pdMS_TO_TICKS(10000)) == ESP_OK) {
                printf("Message received\n");
            } else {
                printf("Failed to receive message\n");
                return;
            }

            //Process received message
            if (message.extd) {
                printf("Message is in Extended Format\n");
            } else {
                printf("Message is in Standard Format\n");
            }
            printf("ID is %d\n", message.identifier);
            if (!(message.rtr)) {
                for (int i = 0; i < message.data_length_code; i++) {
                    printf("Data byte %d = %d\n", i, message.data[i]);
        }
    }
        }

    }


    void start(){
        //idk what to call this method might need a change to make more sense


        //Install TWAI driver
        if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
            printf("Driver installed\n");
        } else {
            printf("Failed to install driver\n");
            return;
        }

        //Start TWAI driver
        if (twai_start() == ESP_OK) {
            printf("Driver started\n");
        } else {
            printf("Failed to start driver\n");
            return;
        }
    }

    //method to read data from sensor and add it to message to be transmitted
    virtual char createMessage(uint8_t* data ) = 0 ;
    

    void transmitMessage(){
        createMessage(&message.data);
        //Queue message for transmission
        if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
            printf("Message queued for transmission\n");
        } else {
            printf("Failed to queue message for transmission\n");
        }
    }

    };