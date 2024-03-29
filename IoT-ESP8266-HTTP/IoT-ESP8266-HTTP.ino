#include <ESP8266WiFi.h>
#include <base64.h>
#include "src/sha256.h"
#include <ArduinoJson.h>
#include <WiFiClientSecureBearSSL.h>
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);


const char* ssid = "*******";
const char* password = "******";

const char* host = "temperatuursensoren.azure-devices.net";
const char* deviceId = "dht11_http";
const char* apiVersion = "2016-11-14";

const char* policyName = "iothubowner";
const char* sharedAccessKey = "*******";

String generateSasToken(String uri, String key, String policyName, int expiry) {
  unsigned long epochTime = time(nullptr);
  String expiryTime = String(epochTime + expiry);
  String stringToSign = uri + "\n" + expiryTime;
  Serial.println(stringToSign);

  char signature[HASH_LENGTH];
  Sha256 hash;
  hash.initHmac((const uint8_t*) key.c_str(), key.length());
  hash.resultHmac();

  String encodedSignature = base64::encode((const uint8_t*) signature, HASH_LENGTH);
  String token = "SharedAccessSignature sr=" + uri + "&sig=" + encodedSignature + "&se=" + expiryTime;
  if (policyName != "") {
    token += "&skn=" + policyName;
  }
  return token;
}



void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  dht.begin();
}

void loop() {
  String sasToken = generateSasToken(host, sharedAccessKey, policyName, 3600);
  Serial.println(sasToken);

  String url = "https://" + String(host) + "/devices/" + String(deviceId) + "/messages/events?api-version=" + String(apiVersion);

  WiFiClientSecure client;
    if (!client.connect(host, 443)) {
      Serial.println("Connection failed");
      return;
    }
  
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  // float humidity = 

  String signal = "ok";
  String payload = "{\"Temperature\":" + String(temperature) + ", \"Humidity\":" + String(humidity) + ", \"Signal\":\"" + signal + "\"}";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Authorization: " + sasToken + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: " + payload.length() + "\r\n" +
               "\r\n" + payload + "\r\n");

  Serial.println("Sending data...");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  client.stop();

  delay(5000);
}

