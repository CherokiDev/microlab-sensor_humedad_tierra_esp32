# 🌱 microlab-sensor_humedad_tierra_esp32

Este proyecto forma parte de la colección [Microlab](https://github.com/cherokidev/microlab) y permite medir la humedad del suelo usando un ESP32 y un sensor analógico de humedad.

---

## 📌 Objetivo

Detectar el nivel de humedad en la tierra para posibles aplicaciones de riego automático, alertas o simplemente monitoreo ambiental.

---

## 🧰 Componentes utilizados

- ESP32 Dev Module
- Sensor de humedad de suelo analógico

---

## 🔌 Wiring / Conexiones

| Sensor de humedad | ESP32   |
| ----------------- | ------- |
| VCC               | 3.3V    |
| GND               | GND     |
| A0 (Salida)       | GPIO 34 |

> Válvula o relé conectado a GPIO 2 (con transistor y protección adecuada).

---

## 🚀 Características principales

- Lectura periódica de humedad del suelo (cada 5 minutos).
- Control automático de una válvula de riego: se abre si la humedad es ≤40%, se cierra si es >40%.
- Publicación de datos y logs por MQTT.
- Los mensajes de log incluyen la fecha y hora actual (sincronizada por NTP, ajustada +2 horas respecto a UTC).
- Toda la lógica y logs también disponibles por el monitor serie.

---

## 📜 Licencia

MIT © [cherokidev](https://github.com/cherokidev)
