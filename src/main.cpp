#include <Arduino.h>
#include "FS.h"
#include "ESPAsyncWebServer.h"
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char *ssid = "FFBikes";
const char *password = "password123";

const char *PARAM_MESSAGE = "color";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  boolean result = WiFi.softAP(ssid, password);
  if (result == true)
  {
    Serial.println("Ready");
  }
  else
  {
    Serial.println("Failed!");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/lights", HTTP_GET, [](AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(PARAM_MESSAGE))
    {
      message = request->getParam(PARAM_MESSAGE)->value();
    }
    else
    {
      message = "No message sent";
    }
    request->send(200, "text/plain", "Hello, GET: " + message);
  });

  // Start server
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}
