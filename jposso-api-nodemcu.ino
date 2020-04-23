
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

// WiFi
#define WIFI_SSID "LeydiG"
#define WIFI_PASSWORD "L19852001"

// Nombre del dispositivo
String device = "nodemcu-1";

// Variable Temporal
float n = -5;

void setup() {
  // Abre el puerto Serie
  Serial.begin(9600);

  // Conecta a la red WiFi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado, mi IP es: ");
  Serial.println(WiFi.localIP());

  // Se conecta y autentica con Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  // Nombre del Sensor
  String sensorName = "ds18b20-5";

  // Inicializa la clase JSON
  DynamicJsonBuffer jsonBuffer;

  // Estructura la informacion
  String input = "{\"timestamp\": {\".sv\": \"timestamp\"} ,\"value\":"+String(n)+"}";
  JsonObject& data = jsonBuffer.parseObject(input);
  
  // Agrega un nuevo valor para /device/sensorName
  String name = Firebase.push(device + "/" + sensorName, data);
  
  // Si se produce errores
  if (Firebase.failed()) {
      Serial.print("pushing " + device + "/" + sensorName + " failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.println("pushed: /" + device + "/" + sensorName);

  // Actualiza valor unico en el sensor
  Firebase.setFloat(device + "/" + sensorName + "/value", n);
  
  // Si se produce errores
  if (Firebase.failed()) {
      Serial.print("setting " + device + "/" + sensorName + "/value" + " failed:");
      Serial.println(Firebase.error());  
      return;
  }

  n = n + 0.5;
  delay(5000);
}
