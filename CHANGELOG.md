# 📜 CHANGELOG

Todas las novedades y mejoras del proyecto `microlab-sensor_humedad_tierra_esp32`.

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
