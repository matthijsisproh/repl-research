# 1 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino"
# 2 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2
# 3 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2
# 4 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2
# 5 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2
# 6 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2
# 7 "C:\\Users\\MatthijsKoelewijnDen\\Documents\\Github\\repl-research\\IoT-ESP8266-HTTP\\IoT-ESP8266-HTTP.ino" 2





DHT dht(2, DHT11 /* DHT 11*/);


const char* ssid = "Handelsweg 9";
const char* password = "Hb9Bu76%Rad";

const char* host = "temperatuursensoren.azure-devices.net";
const char* deviceId = "dht11_http";
const char* apiVersion = "2016-11-14";

const char* policyName = "iothubowner";
const char* sharedAccessKey = "1vUD4bATBx05zqv7WX5JzY+OJo/ObLdOJ4ZTBWX4F0Q=";

String generateSasToken(String uri, String key, String policyName, int expiry) {
  unsigned long epochTime = time(nullptr);
  String expiryTime = String(epochTime + expiry);
  String stringToSign = uri + "\n" + expiryTime;
  Serial.println(stringToSign);

  char signature[32];
  Sha256 hash;
  hash.initHmac((const uint8_t*) key.c_str(), key.length());
  hash.resultHmac();

  String encodedSignature = base64::encode((const uint8_t*) signature, 32);
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
