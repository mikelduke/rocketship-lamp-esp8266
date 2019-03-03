#include <EEPROM.h>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 13

#define DELAY 40

// Define the array of leds
CRGB leds[NUM_LEDS];

/*************** WIFI ***********/

#include <ESP8266WiFi.h>
// #include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "WiFiConfig.h"
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


/************** PATTERNS ***************/
uint8_t currentPatternIndex = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

#include "Patterns.h"

PatternAndNameList patterns = {
  { ring,                  "Ring" },
  { ring_two,              "Ring Type 2" },
  { huefade,               "Hue Fade" }
};

const uint8_t patternCount = ARRAY_SIZE(patterns);

/******************* FUNCTIONS **********************/

void setup() {
  Serial.begin(115200);
  Serial.println("Rocket Lamp");

  EEPROM.begin(512);
  loadSettings();

  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);

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
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  server.on("/pattern", HTTP_POST, []() {
    String value = server.arg("value");
    setPattern(value.toInt());
    sendInt(currentPatternIndex);
  });

  server.on("/pattern", HTTP_GET, []() {
    sendInt(currentPatternIndex);
  });

  server.on("/patterns", HTTP_GET, []() {
    String json = "{\"patterns\":[";

    for (uint8_t i = 0; i < patternCount; i++) {
      json += "{\"id\":" + String(i) + ",\"name\":\"" + patterns[i].name + "\"}";
      if (i < patternCount - 1)
        json += ",";
    }
    json += "]}";

    sendString(json);
  });

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

void setPattern(uint8_t value) {
  if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

  EEPROM.write(0, currentPatternIndex);
  EEPROM.commit();

  Serial.printf("Pattern: %d\n", currentPatternIndex);
}

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void sendInt(uint8_t value) {
  sendString(String(value));
}

void sendString(String value) {
  server.send(200, "text/plain", value);
}

void sendJson(String value) {
  server.send(200, "application/json", value);
}
