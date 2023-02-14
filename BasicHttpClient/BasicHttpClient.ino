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

int IN1_pin = 16;
int IN2_pin = 14;
int IN3_pin = 12;
int IN4_pin = 13;

int ENA_pin = 5;
int ENB_pin = 4;

void setup() {
  Serial.printf("Setting Up");
  pinMode(ENA_pin, OUTPUT);
  pinMode(ENB_pin, OUTPUT);

  pinMode(IN1_pin, OUTPUT);
  pinMode(IN2_pin, OUTPUT);
  pinMode(IN3_pin, OUTPUT);
  pinMode(IN4_pin, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
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
  digitalWrite(LED_BUILTIN, LOW);
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    Serial.printf("Connected Succesfully");
    WiFiClient client;
    HTTPClient http;
    while (1) {
      if (http.begin(client, "http://192.168.137.1:4000")) {  // HTTP
        digitalWrite(LED_BUILTIN, HIGH);
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
            int IN1 = res["IN1"];
            int IN2 = res["IN2"];
            int IN3 = res["IN3"];
            int IN4 = res["IN4"];
            int ENA = res["ENA"];
            int ENB = res["ENB"];

            digitalWrite(IN1_pin, IN1);
            digitalWrite(IN2_pin, IN2);
            digitalWrite(IN3_pin, IN3);
            digitalWrite(IN4_pin, IN4);
            analogWrite(ENA_pin, ENA);
            analogWrite(ENB_pin, ENB);
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
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Not Connected Trying again...");
  }
  delay(1000);
}
