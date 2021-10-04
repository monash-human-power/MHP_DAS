from umqtt.simple import MQTTException, MQTTClient
import uasyncio as asyncio


class Client:
    def __init__(self, client_id, broker_address):
        """
        Initialises the MQTT Client
        :param client_id: The unique client id used to initiate an MQTTClient class
        :param broker_address: A string holding domain name or IP address of the broker to connect to, to send and
                            receive data.
        """
        self.keep_alive_interval = 60
        self.client = MQTTClient(client_id, broker_address, keepalive=self.keep_alive_interval)
        self.mqtt_broker = broker_address
        self.connected = False

    async def connect_and_subscribe(self, topics_to_subscribe, callback_func):
        """
        Connects to the MQTT broker and subscribes to each topic in 'topics_to_subscribe' using QoS = 1
        :param topics_to_subscribe: An array of topics to subscribe to.
                                    Each element must be a string or byte literal (the latter is preferred)
        :param callback_func: The function to be called whenever a message from the subscribed topic is received.
        """
        self.client.set_callback(callback_func)

        # Connect to MQTT broker
        while not self.connected:
            try:
                print(
                    "Attempting connection to MQTT broker at {}...".format(
                        self.mqtt_broker
                    )
                )
                self.client.connect()
                self.connected = True
            except (OSError, MQTTException) as e:
                print("Failed to connect! {}: {}".format(type(e), e))
                print("Reattempting in 5 seconds.")
                await asyncio.sleep(5)

        print("Connected to {}".format(self.mqtt_broker))
        self.connected = True

        # Subscribe to each topic
        for topic in topics_to_subscribe:
            self.client.subscribe(topic, qos=1)
            print("Subscribed to {} topic".format(topic))

        # Start pingreq loop
        asyncio.create_task(self.start_ping_loop())

        return True

    async def start_ping_loop(self):
        """
        Sends a PINGREQ message to the broker at a regular interval so it knows we're
        still connected. The server's response is handled by umqtt automatically.
        """
        while True:
            if self.connected:
                self.client.ping()
            asyncio.sleep(self.keep_alive_interval)


    def _to_bytes_literal(self, data):
        """
        Converts data into a form MQTT can read
        :param data: A string of data to convert to bytes literal
        :return: The bytes literal version of the 'data'
        """
        str_data = str(data)
        return str.encode(str_data)

    def publish(self, topic, data="", retain=False):
        """
        This function takes care of all of the formatting and publishes 'data' on the given topic. Also checks for any
        incoming messages
        :param topic: A string representing the topic to send 'data' to.
        :param data: A string of data to send/publish
        :param retain:
        """
        msg = self._to_bytes_literal(data)
        self.client.publish(topic, msg, retain=retain)

        self.check_for_message()

    def check_for_message(self):
        """
        Checks whether an incoming message from the MQTT broker on the subscribed topics is pending.
        """
        self.client.check_msg()

    def wait_for_message(self):
        """
        This function blocks until a message is received on one of the subscribed topics
        """
        self.client.wait_msg()

    def disconnect(self):
        """
        Disconnect from the broker
        """
        self.connected = False
        self.client.disconnect()
