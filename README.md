# Monitoramento de Temperatura com ESP32, Termopar K e NTC 100k

Este projeto consiste em um sistema de telemetria e aquisição de dados térmicos baseado no microcontrolador ESP32. O sistema monitora temperaturas em tempo real através de uma interface Web, utilizando dois métodos distintos de sensoriamento para cobrir diferentes faixas e aplicações.

O projeto foi desenvolvido como requisito avaliativo para a disciplina de **Sensores e Instrumentação**, ministrada pelo **Prof. [cite_start]Dr. Alexandre Fonseca Jorge** no **IFSP - Câmpus Bragança Paulista**[cite: 48, 55].

## 📋 Funcionalidades

* **Leitura Híbrida:** Monitoramento simultâneo de altas temperaturas (Termopar Tipo K) e temperaturas ambientes/baixas (NTC) [cite: 58-60].
* **Web Server Embarcado:** Interface gráfica acessível via navegador (WiFi) contendo:
    * Dashboard com valores instantâneos.
    * [cite_start]Gráfico em tempo real (plotado via Chart.js)[cite: 33].
    * [cite_start]Indicador visual de status de conexão (LED virtual)[cite: 14].
* **Tratamento de Dados:**
    * [cite_start]Suavização de ruído no NTC através de média de 20 amostras[cite: 6, 292].
    * [cite_start]Aplicação da Equação de Steinhart-Hart para linearização da leitura do termistor[cite: 10, 315].

## 🛠️ Hardware e Materiais

[cite_start]Conforme documentado no relatório do projeto[cite: 62]:

* [cite_start]**Microcontrolador:** ESP32 (DevKit V1)[cite: 64].
* **Sensor 1:** Termopar Tipo K + Módulo Amplificador MAX6675[cite: 63, 65].
* [cite_start]**Sensor 2:** Termistor NTC 100kΩ (Coeficiente Beta ~4092)[cite: 5, 66].
* [cite_start]**Resistor:** 100kΩ (para o divisor de tensão do NTC)[cite: 67].

## 🔌 Pinagem e Conexões

[cite_start]As conexões físicas definidas no firmware são[cite: 3, 4]:

| Componente | Pino ESP32 | Função |
| :--- | :--- | :--- |
| **MAX6675 SO** | GPIO 19 | Dados (MISO) |
| **MAX6675 CS** | GPIO 5 | Chip Select |
| **MAX6675 SCK** | GPIO 18 | Clock |
| **MAX6675 VCC** | 3.3V / 5V | Alimentação |
| **MAX6675 GND** | GND | Terra |
| **NTC 100k** | GPIO 34 | Leitura Analógica (ADC) |

> [cite_start]**Nota sobre o NTC:** O termistor está configurado em um circuito divisor de tensão com o resistor de 100kΩ[cite: 60].

## Pm Teoria Aplicada

### Termopar Tipo K (MAX6675)
Utiliza o efeito Seebeck, onde a diferença de potencial gerada entre dois metais distintos é convertida em temperatura. [cite_start]O módulo MAX6675 realiza a compensação de junta fria e digitaliza o sinal[cite: 92, 436].

### Termistor NTC (Steinhart-Hart)
O NTC altera sua resistência drasticamente com a temperatura. [cite_start]Para converter a resistência lida em temperatura Celsius, o código utiliza uma simplificação da Equação de Steinhart-Hart[cite: 320, 322]:

$$\frac{1}{T} = \frac{1}{T_0} + \frac{1}{\beta} \ln\left(\frac{R}{R_0}\right)$$

Onde:
* $T_0 = 298.15 K$ ($25^\circ C$)
* $R_0 = 100.000 \Omega$
* $\beta = 4092$ (Coeficiente do material)

## 🚀 Como Executar

1.  Instale a **Arduino IDE**.
2.  Instale as bibliotecas necessárias via Gerenciador de Bibliotecas:
    * `MAX6675 library` por Adafruit.
3.  Abra o arquivo `Esp32_Temp_Sensoring.ino`.
4.  [cite_start]**Configuração de Rede:** Altere as linhas 7 e 8 com suas credenciais WiFi[cite: 1, 2]:
    ```cpp
    const char* ssid = "SUA_REDE_WIFI";
    const char* password = "SUA_SENHA_WIFI";
    ```
5.  [cite_start]**Verifique o Resistor:** Certifique-se de que a constante `SERIES_RESISTOR` (linha 16) corresponde ao valor real do resistor usado no divisor de tensão (no código original está calibrado para 260k, ajuste para 100000.0 se estiver usando 100k exatos).
6.  Carregue o código no ESP32.
7.  Abra o Monitor Serial (115200 baud) para visualizar o endereço IP atribuído.
8.  Acesse o IP no navegador para ver o dashboard.

## 👥 Autores

[cite_start]Trabalho realizado pelos alunos [cite: 51-54]:
* **Jonathan A. M. Candido**
* **Samara L. C. Hurtado**
* **Henrique M. Ribeiro**
* **Rafael D. S. Magalhães**

## 📚 Referências
* [Wikipedia: Steinhart–Hart equation](https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation)
* [Adafruit MAX6675 Library](https://github.com/adafruit/MAX6675-library)
