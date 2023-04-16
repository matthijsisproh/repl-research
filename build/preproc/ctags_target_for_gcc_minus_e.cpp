# 1 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

# 6 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino" 2
# 7 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino" 2
# 8 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino" 2
# 9 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino" 2
# 18 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
const char* SCOPE_ID = "0ne00996788";
const char* DEVICE_ID = "mydht11";
const char* DEVICE_KEY = "U50xYdq3yVB/Inw7Nq9rgMBXRhFJqVy14E+1AuT1yCI=";

DHT dht(2, DHT11 /* DHT 11*/);

void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo);
# 26 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino" 2

void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo) {
  // ConnectionStatus
  if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0) {
    do { Serial.printf("  - "); Serial.printf("Is connected ? %s (%d)", callbackInfo->statusCode == 0x40 ? "YES" : "NO", callbackInfo->statusCode); Serial.printf("\r\n"); } while (0)

                                         ;
    isConnected = callbackInfo->statusCode == 0x40;
    return;
  }

  // payload buffer doesn't have a null ending.
  // add null ending in another buffer before print
  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0) {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }

  do { Serial.printf("  - "); Serial.printf("- [%s] event was received. Payload => %s\n", callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY"); Serial.printf("\r\n"); } while (0)
                                                                              ;

  if (strcmp(callbackInfo->eventName, "Command") == 0) {
    do { Serial.printf("  - "); Serial.printf("- Command name was => %s\r\n", callbackInfo->tag); Serial.printf("\r\n"); } while (0);
  }
}

void setup() {
  Serial.begin(9600);

  connect_wifi("Handelsweg 9", "Hb9Bu76%Rad");
  connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);

  if (context != __null) {
    lastTick = 0; // set timer in the past to enable first telemetry a.s.a.p
  }
   dht.begin();
}


void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();


  if (isConnected) {

    // unsigned long ms = millis();
    // if (ms - lastTick) {  // send telemetry every 10 seconds
      char msg[64] = {0};
      int pos = 0, errorCode = 0;

      // lastTick = ms;
      // if (loopId++ % 2 == 0) {  // send telemetry
        pos = snprintf(msg, sizeof(msg) - 1, "{\"Temperature\": %f}",
                       t);
        errorCode = iotc_send_telemetry(context, msg, pos);

        pos = snprintf(msg, sizeof(msg) - 1, "{\"Humidity\":%f}",
                       h);
        errorCode = iotc_send_telemetry(context, msg, pos);

      // } else {  // send property

      // } 

      msg[pos] = 0;

      if (errorCode != 0) {
        do { Serial.printf("X - Error at %s:%d\r\n\t", "IoT-ESP8266-Azure.ino", 95); Serial.printf("Sending message has failed with error code %d", errorCode); Serial.printf("\r\n"); } while (0);
      }
    }
    else{
    iotc_do_work(context); // do background work for iotc
    iotc_free_context(context);
    context = __null;
    connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);
    }
  }
