from azure.iot.device import IoTHubDeviceClient, Message
import json
import time

# IoT Hub Connection Strings
connection_string = "HostName=temperatuursensors.azure-devices.net;DeviceId=temperature-meter;SharedAccessKey=fjHCcV7yf+9lzKceKf3udCptI43rkLYBbcOpwdhKPWE="
device_id = "temperature-meter"
device_key = ""

# Temperature and Humidity Sensor Measurements
temperature = 25.0
humidity = 60.0

# Create Telemetry Payload
payload = {
    "temperature": temperature,
    "humidity": humidity
}
message = Message(json.dumps(payload))

# Create IoT Hub Client
client = IoTHubDeviceClient.create_from_connection_string(connection_string)

# Connect to IoT Hub
client.connect()

# Send Telemetry Message
try:
    client.send_message(message)
    print("Telemetry data sent successfully!")
except Exception as e:
    print("Error sending telemetry data: ", e)

# Disconnect from IoT Hub
client.disconnect()
