#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "Handelsweg 9";
const char* password = "Hb9Bu76%Rad";
const char* host = "https://smartmeter-afstuderen.azureiotcentral.com";
const int httpsPort = 443;

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setInsecure();
  Serial.print("Connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
}

void loop() {
  String data = "23";
  String request = "POST /api/query?api-version=2022-10-31-preview HTTP/1.1\r\n";
  request += "Host: ";
  request += host;
  request += "\r\n";
  request += "Authorization: SharedAccessSignature sr=https://smartmeter-afstuderen.azureiotcentral.com/devices/1omrl5pqwgb&sig=btJg6et%2FBIEoyaBokCODwxUk+VbMVubX%2F7b6iUpzsWI=&se=3600\r\n";
  request += "Content-Type: application/json\r\n";
  request += "Content-Length: ";
  request += data.length();
  request += "\r\n\r\n";
  request += data;

  Serial.println(request);
  client.print(request);
  delay(5000);
}
