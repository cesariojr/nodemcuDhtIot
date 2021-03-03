
/*
   02/03/2021
   Jose Maria Cesario Jr
   Exemplo IBM Watson IoT Platform
   Hardware: new NodeMCU AMICA + DHT11

   Logica:
   1. efetua conexao com a rede WiFi e cria cliente MQTT
   2. obtem as grandezas de temperatura, umidade do sensor DHT11
   3. conecta no servico IBM Watson IoT Platform
   4. publica a JSON string para o topico

   referencias Bluemix e IoTF: 
   Author: Ant Elder
   https://developer.ibm.com/recipes/tutorials/connect-an-esp8266-with-the-arduino-sdk-to-the-ibm-iot-foundation/
   
   referencias conversao float para string
   http://www.hobbytronics.co.uk/arduino-float-vars
   http://forum.carriots.com/index.php/topic/61-wireless-gardening-with-arduino-cc3000-wifi-modules/page-2
   
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"

//atualize SSID e senha WiFi
const char* ssid = "";
const char* password = "";

#define DHTPIN 2 // D4 PIN NODEMCU = 2 
#define DHTTYPE DHT11   // DHT 11

//D4 only for Lolin board
#define LED_BUILTIN D4

//Atualize os valores de Org, device type, device id e token
#define ORG ""
#define DEVICE_TYPE ""
#define DEVICE_ID ""
#define TOKEN ""

//broker messagesight server
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

float temperatura = 0.0;
float umidade = 0.0;

char temperaturastr[6];
char umidadestr[6];

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

// Initialize WiFi client
WiFiClient wifiClient;

// Initialize MQTT client
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando...");

  Serial.print("Conectando na rede WiFi "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("[INFO] Conectado WiFi IP: ");
  Serial.println(WiFi.localIP());

  pinMode(LED_BUILTIN, OUTPUT);

  dht.begin();
}

void loop() {

  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }

  delay(1000);
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
  Serial.print(F("°C "));
 
  dtostrf(t, 6, 0, temperaturastr);
  dtostrf(h, 6, 0, umidadestr);

  // Conversao Floats para Strings
  char TempString[32];  //  array de character temporario

  // dtostrf( [Float variable] , [Minimum SizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
  dtostrf(t, 2, 1, TempString);
  String temperaturastr =  String(TempString);
  
  dtostrf(h, 2, 2, TempString);
  String umidadestr =  String(TempString);
  
  // Prepara JSON para IOT Platform
  int length = 0;

  String payload = "{\"d\":{\"temperatura\": \"" + String(temperaturastr) + "\",\"umidade\":\"" + String(umidadestr) + "\"}}";

  length = payload.length();
  Serial.print(F("\nData length: "));
  Serial.println(length);

  Serial.print("Sending payload: ");
  Serial.println(payload);

  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("Publish ok");
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  } else {
    Serial.println("Publish failed");
  }
 
  delay(5000);
}
