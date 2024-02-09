import json
import time
from awscrt import mqtt, http
from awsiot import mqtt_connection_builder

# Function to handle received messages on the "receive" topic
def on_receive_message_received(topic, payload, dup, qos, retain, **kwargs):
    print("Received message on topic '{}': {}".format(topic, payload.decode('utf-8')))
    
    return

# Function to subscribe to a specific topic
def subscribe_to_topic(mqtt_connection, topic, qos):
    subscribe_future, _ = mqtt_connection.subscribe(
        topic=topic,
        qos=qos,
        callback=on_receive_message_received,
    )
    subscribe_result = subscribe_future.result()
    print("Subscribed to topic '{}' with {}".format(topic, str(subscribe_result['qos'])))
    return

# Function to publish data to a specific topic
def publish_to_topic(mqtt_connection, topic, json_string, delay, qos):
    while True:
        mqtt_connection.publish(
            topic=topic,
            payload=json_string,
            qos=qos,
        )
        print("Published message to topic '{}': {}".format(topic, json_string))
        time.sleep(delay)  # delay in seconds between messages
        return

# AWS IoT configuration
endpoint = "a2w5rovb23kaby-ats.iot.eu-north-1.amazonaws.com"  # Replace with your AWS IoT endpoint
root_ca_path = "certs/Amazon-root-CA-1.pem"  # Replace with your root CA certificate path
private_key_path = "certs/private.pem.key"  # Replace with your private key path
certificate_path = "certs/device.pem.crt"  # Replace with your device certificate path
client_id = "ESP32_TEST1"  # Replace with your client ID

# Create the proxy options if needed
proxy_options = None

# Create a MQTT connection
mqtt_connection = mqtt_connection_builder.mtls_from_path(
    endpoint=endpoint,
    port=8883,
    cert_filepath=certificate_path,
    pri_key_filepath=private_key_path,
    ca_filepath=root_ca_path,
    client_id=client_id,
    clean_session=False,
        keep_alive_secs=30,
    http_proxy_options=proxy_options,
)

# Connect to AWS IoT
connect_future = mqtt_connection.connect()
connect_future.result()

# Subscribe to the "ESP32_Node_01" topic to receive data from Esp32 node 01
subscribe_to_topic(mqtt_connection, "ESP32_01", mqtt.QoS.AT_LEAST_ONCE)
subscribe_to_topic(mqtt_connection, "AWS_Python", mqtt.QoS.AT_LEAST_ONCE)

count = 1
while True:
    pass
    # print("\ncount",count)
    # json_string = json.dumps({"message": count})
    # publish_to_topic(mqtt_connection, "AWS_Python", json_string, 5, mqtt.QoS.AT_LEAST_ONCE)
    # count += 1
    # time.sleep(4)

# print("Disconnected!")
# disconnect_future = mqtt_connection.disconnect()
# disconnect_future.result()

