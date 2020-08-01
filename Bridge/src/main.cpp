#include <iostream>
#include <iomanip>

#include "mqtt/async_client.h"
#include "mqtt/message.h"

#include "protocol.hpp"
#include "mqtt.hpp"

void receive_callback(mqtt::const_message_ptr message)
{
    std::cout << "Parsed packet!" << std::endl
        << "\tQoS:      " << message->get_qos() << std::endl
        << "\tRetained: " << message->is_retained() << std::endl
        << "\tTopic:    " << message->get_topic() << std::endl
        << "\tPayload:  " << message->get_payload() << std::endl;
}

int main()
{
    std::cout << "Hello world!" << std::endl;


    ////////////// MQTT //////////////

    MqttBridgeClient mqttClient;
    mqttClient.set_on_message(receive_callback);

    ////////////// PROTOCOL //////////////

    TxProtocol tx;
    RxProtocol rx;

    auto packets = tx.packMessage(
        mqtt::make_message("test_topic", "Hello world!", 0, false)
    );

    if (auto message = rx.receivePacket(packets[0]))
        mqttClient.publish(*message);

    while (std::cin.get() != 'q')
        ;

    return 0;
}
