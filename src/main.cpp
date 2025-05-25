#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <time.h>

const int sensorPin = 34; // Pin analógico del sensor de humedad

WiFiClient espClient;
PubSubClient client(espClient);

const char *device_id = "sensor_humedad_01";
const int valvePin = 2; // Pin para el control del transistor

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Conectando a MQTT...");
    if (client.connect(device_id, MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("Conectado!");
      client.publish(("sensors/" + String(device_id) + "/status").c_str(), "online");
    }
    else
    {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo...");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");

  // Configura NTP
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("Esperando sincronización NTP...");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("\nHora sincronizada!");

  client.setServer(MQTT_SERVER, MQTT_PORT);
  reconnectMQTT();

  Serial.println("Iniciando lectura del sensor de humedad...");
}

void loop()
{
  if (!client.connected())
  {
    reconnectMQTT();
  }

  int sensorValue = analogRead(sensorPin);
  float humedad = map(sensorValue, 4095, 0, 0, 100);

  // Control directo de la válvula según humedad
  if (humedad <= 40)
  {
    digitalWrite(valvePin, HIGH); // Abre la válvula
  }
  else
  {
    digitalWrite(valvePin, LOW); // Cierra la válvula
  }

  // Obtiene fecha y hora actual (+2 horas)
  time_t now = time(nullptr) + 2 * 3600;
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char timeString[25];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", &timeinfo);

  String payload = "{\"device_id\":\"" + String(device_id) + "\",\"sensor_type\":\"soil_moisture\",\"humidity\":" + String(humedad) + "}";
  client.publish(("sensors/" + String(device_id) + "/data").c_str(), payload.c_str());

  String logMsg = String(timeString) + " | Valor analógico: " + String(sensorValue) + " | Humedad: " + String(humedad) + "% | Válvula: " + (humedad <= 40 ? "ABIERTA" : "CERRADA");
  Serial.println(logMsg);
  client.publish(("sensors/" + String(device_id) + "/log").c_str(), logMsg.c_str());

  delay(300000); // Espera 5 minutos antes de la siguiente lectura
}