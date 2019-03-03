#include <EEPROM.h>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 13

#define DELAY 40

CRGB leds[NUM_LEDS];

#include "LedUtil.h"
#include "HttpServer.h"
#include "Patterns.h"

#include <ESP8266WiFi.h>
#include "WiFiConfig.h" //Define STASSID and STAPSK in WifiConfig.h or remove this and define below
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;


/******************* FUNCTIONS **********************/

void setup() {
  Serial.begin(115200);
  Serial.println("Rocket Lamp");

  EEPROM.begin(512);
  loadSettings();

  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);

  setupWiFi();

  setupHttpServer();

  // setPattern(1); // force override eeprom
}

void loop() {
  server.handleClient();
  patterns[currentPatternIndex].pattern();
}

void loadSettings() {
  currentPatternIndex = EEPROM.read(0);
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patternCount)
    currentPatternIndex = patternCount - 1;

  Serial.printf("loadSettings currentPatternIndex:%d\n", currentPatternIndex);
}

void setupWiFi() {
  flashLeds(CRGB::Red, 2);
  setColor(CRGB::White);

  Serial.printf("Connecting to SSID: %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  flashLeds(CRGB::Blue, 5);
}
