#include <Arduino.h>
#line 1 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include <ESP8266WiFi.h>
#include "src/iotc/common/string_buffer.h"
#include "src/iotc/iotc.h"
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11   // DHT 11


#define WIFI_SSID "Handelsweg 9"
#define WIFI_PASSWORD "Hb9Bu76%Rad"

const char* SCOPE_ID = "0ne00996788";
const char* DEVICE_ID = "mydht11";
const char* DEVICE_KEY = "U50xYdq3yVB/Inw7Nq9rgMBXRhFJqVy14E+1AuT1yCI=";

DHT dht(DHTPIN, DHTTYPE);

void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo);
#include "src/connection.h"

#line 52 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
void setup();
#line 65 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
void loop();
#line 27 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-Azure\\IoT-ESP8266-Azure.ino"
void on_event(IOTContext ctx, IOTCallbackInfo* callbackInfo) {
  // ConnectionStatus
  if (strcmp(callbackInfo->eventName, "ConnectionStatus") == 0) {
    LOG_VERBOSE("Is connected ? %s (%d)",
                callbackInfo->statusCode == IOTC_CONNECTION_OK ? "YES" : "NO",
                callbackInfo->statusCode);
    isConnected = callbackInfo->statusCode == IOTC_CONNECTION_OK;
    return;
  }

  // payload buffer doesn't have a null ending.
  // add null ending in another buffer before print
  AzureIOT::StringBuffer buffer;
  if (callbackInfo->payloadLength > 0) {
    buffer.initialize(callbackInfo->payload, callbackInfo->payloadLength);
  }

  LOG_VERBOSE("- [%s] event was received. Payload => %s\n",
              callbackInfo->eventName, buffer.getLength() ? *buffer : "EMPTY");

  if (strcmp(callbackInfo->eventName, "Command") == 0) {
    LOG_VERBOSE("- Command name was => %s\r\n", callbackInfo->tag);
  }
}

void setup() {
  Serial.begin(9600);

  connect_wifi(WIFI_SSID, WIFI_PASSWORD);
  connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);

  if (context != NULL) {
    lastTick = 0;  // set timer in the past to enable first telemetry a.s.a.p
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
        LOG_ERROR("Sending message has failed with error code %d", errorCode);
      }
    }
    else{
    iotc_do_work(context);  // do background work for iotc
    iotc_free_context(context);
    context = NULL;
    connect_client(SCOPE_ID, DEVICE_ID, DEVICE_KEY);
    }
  }



