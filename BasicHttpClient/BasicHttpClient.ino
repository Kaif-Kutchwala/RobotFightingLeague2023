/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
DynamicJsonDocument res(1024);

void setup() {
  Serial.printf("Setting Up");
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Johnny", "12345678990");
  Serial.printf("Trying to Connect to wifi");
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.printf("Connected Succesfully");
    WiFiClient client;
    HTTPClient http;
    while (1) {
      if (http.begin(client, "http://192.168.137.1:5000")) {  // HTTP
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          // Serial.printf("[HTTP] GET... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = http.getString();
            Serial.println(payload);
            deserializeJson(res, payload);
            float ly = res["ly"];
            float ry = res["ry"];
            Serial.println(ly);
            float led1_brightness = ly * 255;
            float led2_birghtness = ry * 255;
            analogWrite(4, led1_brightness);
            analogWrite(5, led2_birghtness);
          }
        } else {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
      } else {
        Serial.printf("[HTTP} Unable to connect\n");
      }
      delay(200);
    }
  }
  delay(1000);
}
