# Temperature Monitoring and Telemetry System: ESP32, K-Type Thermocouple, and 100k NTC

This repository contains the technical implementation of a high-precision thermal data acquisition and telemetry system based on the ESP32 microcontroller. The system integrates heterogeneous sensors to provide real-time temperature monitoring through an embedded web interface, designed for applications requiring both wide-range industrial sensing and ambient precision.

This project was developed as a formal requirement for the Sensors and Instrumentation course, supervised by Prof. Dr. Alexandre Fonseca Jorge at the Federal Institute of São Paulo (IFSP) - Bragança Paulista Campus.

## 1. Technical Overview

The system architecture is designed to handle dual-channel temperature sensing with distinct physical properties:

* **High-Temperature Channel:** Utilizing a K-Type Thermocouple interfaced via a MAX6675 Cold-Junction Compensated K-Thermocouple-to-Digital Converter.
* **Precision Ambient Channel:** Utilizing a 100kΩ Negative Temperature Coefficient (NTC) thermistor via a 12-bit Analog-to-Digital Converter (ADC) input.

### Key Features
* **Asynchronous Data Streaming:** Real-time data visualization via a web dashboard.
* **Signal Processing:** Implementation of digital filtering (moving average) to mitigate electromagnetic interference (EMI) and ADC quantization noise.
* **Embedded Web Server:** A lightweight HTTP server serving a dynamic frontend with real-time charting capabilities using Chart.js.
* **Non-Linearity Compensation:** Mathematical linearization of thermistor resistance through the Steinhart-Hart model.

## 2. Hardware Architecture and Specifications

### 2.1 Component List
* **Microcontroller:** ESP32 (Xtensa® Dual-Core 32-bit LX6) featuring integrated Wi-Fi and 12-bit SAR ADC.
* **Thermocouple Interface:** MAX6675 (SPI-compatible) providing 0.25°C resolution and cold-junction compensation.
* **Analog Sensor:** 100kΩ NTC Thermistor (Beta Coefficient ≈ 4092).
* **Reference Resistor:** 100kΩ high-precision resistor (used in a voltage divider configuration).

### 2.2 Electrical Connections (Pin Mapping)

The hardware interface is defined as follows:

| Component | ESP32 GPIO | Logic Function | Description |
| :--- | :--- | :--- | :--- |
| **MAX6675 SO** | GPIO 19 | MISO | Serial Data Output |
| **MAX6675 CS** | GPIO 5 | SS | Chip Select (Active Low) |
| **MAX6675 SCK** | GPIO 18 | SCK | Serial Clock |
| **NTC 100k** | GPIO 34 | ADC1_CH6 | Analog Input (12-bit) |
| **VCC** | 3.3V | Power | System Logic Voltage |
| **GND** | GND | Ground | Common Reference |

## 3. Mathematical Modeling and Signal Processing

### 3.1 NTC Linearization
The NTC resistance $R$ is determined via a voltage divider. To convert the resistance into absolute temperature (Kelvin), the system implements the Steinhart-Hart simplification (B-parameter equation):

$$\frac{1}{T} = \frac{1}{T_0} + \frac{1}{\beta} \ln\left(\frac{R}{R_0}\right)$$

Where:
* $T_0$: Reference temperature (298.15 K / 25°C).
* $R_0$: Nominal resistance at $T_0$ (100,000 Ω).
* $\beta$: Material-specific Beta coefficient (4092).
* $R$: Measured resistance derived from the ADC value.

### 3.2 Noise Mitigation
To ensure signal integrity, the firmware implements an oversampling and averaging algorithm. For each NTC reading cycle, the system performs 20 successive samples at a defined sampling frequency to compute a arithmetic mean, effectively acting as a low-pass filter to reject high-frequency noise.

## 4. Software Implementation

### 4.1 Backend
The firmware is written in C++ using the Arduino framework for ESP32. It manages:
* Wi-Fi Station (STA) mode connection.
* SPI communication protocol for the MAX6675.
* HTTP request handling for the root dashboard and JSON data endpoints.

### 4.2 Frontend
The embedded UI is built with HTML5, CSS3, and JavaScript. It utilizes:
* **Chart.js:** To render a dynamic time-series graph of temperature fluctuations.
* **AJAX/Fetch API:** For asynchronous data polling from the ESP32 without requiring a page refresh.

## 5. Deployment Instructions

### 5.1 Environment Setup
1.  Install the **Arduino IDE** (or PlatformIO).
2.  Install the ESP32 Board Manager.
3.  Install the required libraries via the Library Manager:
    * `MAX6675 library` by Adafruit.

### 5.2 Configuration
1.  Open `Esp32_Temp_Sensoring.ino`.
2.  Modify the network credentials (lines 7-8):
    ```cpp
    const char* ssid = "YOUR_WIFI_SSID";
    const char* password = "YOUR_WIFI_PASSWORD";
    ```
3.  **Calibration:** Verify the `SERIES_RESISTOR` constant (line 16). If using a standard 100kΩ resistor, ensure it is set to `100000.0`. If manual calibration is required based on multimeter measurements, adjust accordingly.

### 5.3 Execution
1.  Compile and flash the firmware to the ESP32.
2.  Initialize the Serial Monitor at `115200 baud`.
3.  Once connected, the ESP32 will output its local IP address.
4.  Enter the IP address into any web browser on the same local network to access the telemetry dashboard.

## 6. Project Contributors

* **Jonathan A. M. Candido**
* **Samara L. C. Hurtado**
* **Henrique M. Ribeiro**
* **Rafael D. S. Magalhães**

## 7. References

* Steinhart, J. S., & Hart, S. R. (1968). Calibration curves for thermistors. Deep Sea Research and Oceanographic Abstracts.
* ESP32 Technical Reference Manual.
* Adafruit MAX6675 Library Documentation.
