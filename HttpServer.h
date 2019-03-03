#include "Patterns.h"

#include <ESP8266WebServer.h>

extern ESP8266WebServer server(80);

void handleRoot() {
  // server.send(200, "text/plain", "hello from esp8266!");

  String page = "<html>\
  <head>\
    <title>Rocket Lamp</title>\
  </head>\
  <body>\
    <h1>Rocket Lamp!</h1>\
    <hr />\n";
  page += "<strong>Current Mode:</strong> ";
  page += patterns[currentPatternIndex].name;
  page += "<br />\n";
  for (uint8_t i = 0; i < patternCount; i++) {
    page += "<a href='/set/pattern?value=" + String(i) + "'>" + patterns[i].name + "</a><br />\n";
  }
  page += "</body></html>";
  server.send(200, "text/html", page);
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
  server.on("/", handleRoot);

  server.onNotFound([]() {
    String message = "Error: Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\n";
    server.send(404, "text/plain", message);
  });
  
  server.on("/pattern", HTTP_POST, []() {
    String value = server.arg("value");
    setPattern(value.toInt());
    
    String json = "{\"id\":" + String(currentPatternIndex) + ",\"name\":\"" + patterns[currentPatternIndex].name + "\"}";
    sendJson(json);
  });

  server.on("/set/pattern", HTTP_GET, []() {
    String value = server.arg("value");
    setPattern(value.toInt());
    server.sendHeader("Location", String("/"), true);
    server.send ( 303, "text/plain", "");
  });

  server.on("/pattern", HTTP_GET, []() {
    String json = "{\"id\":" + String(currentPatternIndex) + ",\"name\":\"" + patterns[currentPatternIndex].name + "\"}";
    sendJson(json);
  });

  server.on("/patterns", HTTP_GET, []() {
    String json = "{\"patterns\":[";

    for (uint8_t i = 0; i < patternCount; i++) {
      json += "{\"id\":" + String(i) + ",\"name\":\"" + patterns[i].name + "\"}";
      if (i < patternCount - 1)
        json += ",";
    }
    json += "]}";

    sendJson(json);
  });

  server.begin();
  Serial.println("HTTP server started");
}
