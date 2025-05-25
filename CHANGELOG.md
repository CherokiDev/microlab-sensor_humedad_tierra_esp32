# 📜 CHANGELOG

Todas las novedades y mejoras del proyecto `microlab-sensor_humedad_tierra_esp32`.

---

## [v1.1.1] - 2025-05-25

### Añadido

- Los mensajes de log ahora incluyen la fecha y hora actual (sincronizada por NTP, ajustada +2 horas respecto a UTC).

---

## [v1.1.0] - 2025-05-25

### Añadido

- Publicación de mensajes de log por MQTT en el topic `sensors/<device_id>/log`.
- Mensajes de log enviados también por el monitor serie.
- Lógica de control de la válvula: cuando la humedad baja del 40%, la válvula se abre durante 5 minuto, ignorando el valor del sensor durante ese tiempo. Si vuelve a bajar de 40% tras cerrarse, se repite el ciclo.
- Todos los mensajes de datos se publican en formato JSON en el topic `sensors/<device_id>/data`.

### Eliminado

- Eliminado todo el código relacionado con el display OLED.

---

## [v1.0.1] - 2025-04-13

### Añadido

- LED indicador que se enciende si la humedad baja de cierto umbral.
- Lectura del sensor promediada para evitar lecturas inestables.

### Cambiado

- Ajustado el pin de entrada del sensor de GPIO 34 a GPIO 35 para mejor estabilidad.

---

## [v1.0.0] - 2025-04-13

### Añadido

- Lectura de humedad del suelo con sensor analógico.
- Proyecto base en PlatformIO para ESP32.
- Visualización de lectura por el monitor serie.
- Estructura básica del proyecto con `README.md` y wiring inicial.
