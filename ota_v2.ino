#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>
#include <Adafruit_NeoPixel.h>

// WiFi credentials
const char* ssid = "iPhone Holodof";
const char* password = "11111111";

// GitHub OTA details
const char* update_url = "https://raw.githubusercontent.com/YOUR_USERNAME/YOUR_REPO/main/firmware.bin";

// Pin for RGB LED on ESP32-S3
#define RGB_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel strip(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

const char* current_version = "v2.0.0";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 OTA Test v2.0.0 - SUCCESS!");

  strip.begin();
  strip.setBrightness(50);
  strip.show();

  connectToWiFi();
}

void loop() {
  // Blink GREEN (indicates update success)
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
  delay(500);
  
  strip.setPixelColor(0, strip.Color(0, 0, 0));
  strip.show();
  delay(500);

  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 10000) {
    lastCheck = millis();
    checkForUpdates();
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void checkForUpdates() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Checking for updates...");
    WiFiClientSecure client;
    client.setInsecure(); 
    client.setTimeout(12000 / 1000);

    t_httpUpdate_return ret = httpUpdate.update(client, update_url);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
  }
}
