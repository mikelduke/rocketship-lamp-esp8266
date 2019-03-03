#include "Patterns.h"
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

extern ESP8266WebServer server(80);

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

void sendString(String value) {
  server.send(200, "text/plain", value);
}

void sendInt(uint8_t value) {
  sendString(String(value));
}

void sendJson(String value) {
  server.send(200, "application/json", value);
}

void setupHttpServer() {
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
}