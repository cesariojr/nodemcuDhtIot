
/*
   01/03/2021
   Jose Maria Cesario Jr.
   Adaptado exemplo Adafruit DHT Sensor Library, DHTtester
   Exemplo NodeMCU Amica + DHT 11
*/

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "DHT.h"


#define DHTPIN 2 // D4 PIN NODEMCU = 2 
#define DHTTYPE DHT11   // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando...");
  dht.begin();
}

void loop() {

 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Celsius. Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Umidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.println(F("°C "));

  delay(1000);
}
