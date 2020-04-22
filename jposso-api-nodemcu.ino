
//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

// https://github.com/bblanchon/ArduinoJson/tree/5.x
// https://github.com/FirebaseExtended/firebase-arduino

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

// Set these to run example.
#define FIREBASE_HOST "jposso-iot-dashboard.firebaseio.com"
#define FIREBASE_AUTH "gLoK8I6JkREQqSbXFg7Lea1sRz2WFuRGfadsIcus"
#define WIFI_SSID "Leydig"
#define WIFI_PASSWORD "94232607."

String device = "nodemcu-1";
int n = 0;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  String sensorName = "ds18b20-5";

  DynamicJsonBuffer jsonBuffer;

  JsonObject& temperatureObject = jsonBuffer.createObject();
  JsonObject& tempTime = temperatureObject.createNestedObject("timestamp");
  temperatureObject[".sv"] = "timestamp";

  String input = "{\"timestamp\": {\".sv\": \"timestamp\"} ,\"value\":"+String(n)+"}";
  JsonObject& data = jsonBuffer.parseObject(input);
  
  // append a new value to /logs
  String name = Firebase.push(device + "/" + sensorName, data);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing " + device + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println("");

  // update value
  Firebase.setFloat(device + "/" + sensorName + "/value", n);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /value failed:");
      Serial.println(Firebase.error());  
      return;
  }

  n = n + 1;
  delay(5000);
}
