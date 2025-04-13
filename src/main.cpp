#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h" // Archivo de configuración con credenciales WiFi y MQTT

// Configuración del display OLED
#define SCREEN_WIDTH 128 // Ancho del display en píxeles
#define SCREEN_HEIGHT 64 // Alto del display en píxeles
#define OLED_RESET -1    // Reset por hardware no usado en este caso
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin donde conectamos la salida analógica del sensor
const int sensorPin = 34; // Pin analógico del ESP32

// Configuración de MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Identificador único del dispositivo
const char *device_id = "sensor_humedad_01";

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Conectando a MQTT...");
    if (client.connect(device_id, MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("Conectado!");
      // Publicar mensaje de estado inicial
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
  // Inicializamos el monitor serie
  Serial.begin(115200);

  // Configuramos el pin del sensor como entrada
  pinMode(sensorPin, INPUT);

  // Inicializamos el display OLED
  if (!display.begin(0x3C, 0x3C))
  { // Dirección I2C típica: 0x3C
    Serial.println("Error al inicializar el display OLED");
    while (true)
      ; // Detenemos el programa si falla
  }

  // Limpiamos el buffer del display
  display.clearDisplay();
  display.setTextSize(1);              // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0);             // Posición inicial del texto
  display.println("Iniciando...");
  display.display(); // Mostramos el mensaje inicial
  delay(2000);

  // Conexión WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi!");

  // Configuración del cliente MQTT
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

  // Leemos el valor analógico del sensor
  int sensorValue = analogRead(sensorPin);

  // Convertimos el valor a un rango de 0 a 100% (invertido)
  float humedad = map(sensorValue, 4095, 0, 0, 100);

  // Crear mensaje JSON
  String payload = "{\"device_id\":\"" + String(device_id) + "\",\"sensor_type\":\"soil_moisture\",\"humidity\":" + String(humedad) + "}";

  // Publicar datos en MQTT
  client.publish(("sensors/" + String(device_id) + "/data").c_str(), payload.c_str());

  // Mostramos los valores en el monitor serie
  Serial.print("Valor analógico: ");
  Serial.print(sensorValue);
  Serial.print(" | Humedad: ");
  Serial.print(humedad);
  Serial.println("%");

  // Mostramos los valores en el display OLED
  display.clearDisplay(); // Limpiamos el contenido anterior
  display.setCursor(0, 0);
  display.println("Sensor de Humedad");
  display.setCursor(0, 20);
  display.print("Analogico: ");
  display.println(sensorValue);
  display.setCursor(0, 40);
  display.print("Humedad: ");
  display.print(humedad);
  display.println("%");
  display.display(); // Actualizamos el contenido del display

  // Esperamos 5 segundos
  delay(5000);
}