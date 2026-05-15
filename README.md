<div align="center">
  
# Temperature Monitoring and Telemetry System

<p align="center">
  <img src="https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif" />
  <img src="https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=cplusplus" />
  <img src="https://img.shields.io/badge/IDE-Arduino_IDE-00979D?style=for-the-badge&logo=arduino" />
  <img src="https://img.shields.io/badge/Sensors-K--Type_Thermocouple-important?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Web_Interface-Chart.js-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Architecture-IoT-success?style=for-the-badge" />
</p>

<p align="center">
  Academic project developed for the Sensors and Instrumentation course<br>
  IFSP — Bragança Paulista Campus
</p>

---

## Overview

This project implements a real-time temperature monitoring and telemetry system using the ESP32 microcontroller. The system combines two different sensing methods to provide both high-temperature industrial measurements and precise ambient temperature acquisition.

The first sensing channel uses a K-Type thermocouple connected through a MAX6675 digital converter, while the second channel uses a 100kΩ NTC thermistor connected directly to the ESP32 ADC.

An embedded web server hosted on the ESP32 provides a lightweight dashboard for live monitoring, displaying sensor data through dynamic charts and asynchronous updates over Wi-Fi.

---

## Features

- Dual-channel temperature monitoring
- Real-time telemetry dashboard
- Embedded web server hosted on ESP32
- Dynamic temperature plotting with Chart.js
- ADC noise reduction using moving average filtering
- NTC linearization using the Steinhart-Hart equation
- Wi-Fi monitoring without external software
- SPI communication with MAX6675

---

## Technologies

| Technology | Description |
|---|---|
| C++ | Main firmware language |
| ESP32 | Main microcontroller |
| Arduino IDE | Development environment |
| HTML5 / CSS3 | Embedded frontend |
| JavaScript | Real-time updates |
| Chart.js | Temperature graph rendering |
| MAX6675 | Thermocouple interface |
| Wi-Fi | Local telemetry communication |

---

## Hardware Architecture

The ESP32 operates as the central processing unit responsible for sensor acquisition, data processing, and web communication.

A MAX6675 module handles the K-Type thermocouple through SPI communication, providing cold-junction compensation and digital temperature conversion. Ambient temperature sensing is performed using a 100kΩ NTC thermistor configured in a voltage divider circuit connected to a 12-bit ADC channel.

The system also implements averaging techniques to reduce ADC noise and improve signal stability during acquisition.

---

## Pin Mapping

| Component | ESP32 GPIO | Function |
|---|---|---|
| MAX6675 SO | GPIO 19 | MISO |
| MAX6675 CS | GPIO 5 | Chip Select |
| MAX6675 SCK | GPIO 18 | SPI Clock |
| NTC 100k | GPIO 34 | ADC Input |
| VCC | 3.3V | Power |
| GND | GND | Ground |

---

## Signal Processing

The thermistor temperature calculation uses the Steinhart-Hart simplification based on the Beta parameter equation.

:contentReference[oaicite:0]{index=0}

Where:

- \(T_0\) = 298.15 K (25°C)
- \(R_0\) = 100000 Ω
- \(\beta\) = 4092
- \(R\) = Measured thermistor resistance

To improve measurement stability, the firmware performs multiple ADC samples and computes their arithmetic mean, reducing quantization noise and electromagnetic interference.

---

## Software Architecture

### Backend

The firmware was developed in C++ using the Arduino framework for ESP32. It manages:

- Wi-Fi connection
- SPI communication
- Sensor acquisition
- HTTP request handling
- JSON telemetry endpoints

### Frontend

The web interface uses HTML, CSS, and JavaScript running directly on the ESP32. Temperature data is updated asynchronously using the Fetch API, while Chart.js is responsible for rendering real-time graphs.

---

## Required Libraries

```cpp
#include <WiFi.h>
#include <SPI.h>
#include <max6675.h>
```

---

## Running the Project

1. Install Arduino IDE or PlatformIO.
2. Install the ESP32 board package.
3. Install the MAX6675 library.
4. Open `Esp32_Temp_Sensoring.ino`.
5. Configure your Wi-Fi credentials:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

6. Compile and upload the firmware to the ESP32.
7. Open the Serial Monitor at `115200 baud`.
8. Access the IP address displayed by the ESP32 from any browser on the same network.

---

## Calibration

The NTC thermistor uses a 100kΩ reference resistor in a voltage divider configuration.

```cpp
const float SERIES_RESISTOR = 100000.0;
```

If necessary, this value can be adjusted according to multimeter measurements for improved accuracy.

---

## Applications

This project can be applied to industrial thermal monitoring, laboratory instrumentation, environmental telemetry, embedded sensing systems, and educational IoT experiments involving real-time data acquisition.

---

## Contributors

- Jonathan A. M. Candido
- Samara L. C. Hurtado
- Henrique M. Ribeiro
- Rafael D. S. Magalhães

---

## Institution

Federal Institute of Education, Science and Technology of São Paulo — IFSP  
Bragança Paulista Campus  
Control and Automation Engineering

---

## References

- Steinhart, J. S., & Hart, S. R. — Calibration curves for thermistors
- ESP32 Technical Reference Manual
- Adafruit MAX6675 Documentation
