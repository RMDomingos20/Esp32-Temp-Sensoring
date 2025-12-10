#include <WiFi.h>
#include <WebServer.h>
#include <max6675.h>

// ---------------------------
// CONFIGURAÇÕES DO WIFI
// ---------------------------
const char* ssid = "test";      // <-- NOME WIFI
const char* password = "internet"; // <-- SENHA WIFI

WebServer server(80);

// ---------------------------
// PINOS
// ---------------------------
int thermoSO = 19;
int thermoCS = 5;
int thermoSCK = 18;
MAX6675 thermocouple(thermoSCK, thermoCS, thermoSO);

const int NTC_PIN = 34; 

// ---------------------------
// CALIBRAÇÃO DO NTC 
// ---------------------------
const float SERIES_RESISTOR = 260000.0; 

const float NOMINAL_RESISTANCE = 100000.0; // NTC de 100k
const float NOMINAL_TEMPERATURE = 25.0;
const float B_COEFFICIENT = 4092.0; 

// ---------------------------
// LEITURA NTC COM MÉDIA
// ---------------------------
float readNTCTemp() {
  float totalADC = 0;
  int samples = 20; 

  for (int i = 0; i < samples; i++) {
    totalADC += analogRead(NTC_PIN);
    delay(2); 
  }
  
  float adcValue = totalADC / samples;

  if (adcValue == 0 || adcValue >= 4095) return 0.0;

  float voltage = adcValue / 4095.0 * 3.3;
  
  float resistance = SERIES_RESISTOR * ((3.3 / voltage) - 1.0); 
  
  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE; 
  steinhart = log(steinhart);
  steinhart /= B_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;

  return steinhart;
}

// ---------------------------
// GERA O HTML (COM INDICADOR DE STATUS)
// ---------------------------
String getWebPage() {
  String p = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  p += "<title>Monitor Industrial</title>";
  p += "<style>";
  p += "body { font-family: Arial, sans-serif; background:#111; color:#fff; margin:0; padding:20px; }";
  p += "h2 { text-align: center; margin-bottom: 30px; }";
  
  // STATUS BOX (CSS DO INDICADOR)
  p += ".status-bar { position: absolute; top: 20px; right: 20px; display: flex; align-items: center; gap: 10px; background: #222; padding: 8px 15px; border-radius: 30px; border: 1px solid #444; }";
  p += ".led { width: 12px; height: 12px; border-radius: 50%; transition: all 0.3s; }";
  p += ".led-on { background: #00ff00; box-shadow: 0 0 10px #00ff00; }"; // Verde Brilhante
  p += ".led-off { background: #ff0000; box-shadow: 0 0 10px #ff0000; }"; // Vermelho Brilhante
  p += ".status-text { font-size: 14px; font-weight: bold; }";

  p += ".container { display: flex; flex-wrap: wrap; justify-content: center; align-items: center; gap: 30px; max-width: 1400px; margin: 0 auto; margin-top: 40px; }";
  p += ".temp-circle { background: #1e1e1e; width: 230px; height: 230px; border-radius: 50%; box-shadow: 0 4px 15px rgba(0,0,0,0.5); display: flex; flex-direction: column; justify-content: center; align-items: center; text-align: center; padding: 10px; }";
  p += ".temp-val { font-size: 26px; margin: 5px 0; font-weight: bold; }";
  p += ".label { font-size: 13px; color: #aaa; margin-top: 5px; }";
  p += ".chart-box { flex: 1; min-width: 300px; height: 450px; background: #1e1e1e; padding: 15px; border-radius: 15px; box-shadow: 0 4px 15px rgba(0,0,0,0.5); position: relative; }";
  p += "</style></head><body>";
  
  // HTML DO INDICADOR DE STATUS
  p += "<div class='status-bar'>";
  p += "  <div id='led' class='led led-off'></div>";
  p += "  <div id='statusText' class='status-text' style='color: #aaa;'>Conectando...</div>";
  p += "</div>";

  p += "<h2>Monitor de Temperatura ESP32</h2>";
  p += "<div class='container'>";

  p += "  <div class='temp-circle'>";
  p += "    <div class='label'>Termopar Tipo K</div>"; 
  p += "    <div id='temp1' class='temp-val' style='color: #FF6384;'>-- °C</div>";
  p += "    <hr style='border-color: #333; width: 60%; margin: 10px 0;'>"; 
  p += "    <div class='label'>NTC 100k</div>"; 
  p += "    <div id='temp2' class='temp-val' style='color: #36A2EB;'>-- °C</div>";
  p += "  </div>";

  p += "  <div class='chart-box'>";
  p += "    <canvas id='chart'></canvas>";
  p += "  </div>";
  p += "</div>"; 
  
  p += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  p += "<script>";
  p += "var ctx = document.getElementById('chart').getContext('2d');";
  p += "var chart = new Chart(ctx, { type: 'line', data: { labels: [], datasets: [";
  p += "{ label: 'Termopar Tipo K', data: [], borderColor: '#FF6384', tension: 0.4, borderWidth: 3, pointRadius: 0 },";
  p += "{ label: 'NTC 100k', data: [], borderColor: '#36A2EB', tension: 0.4, borderWidth: 3, pointRadius: 0 }";
  p += "] }, options: { responsive: true, maintainAspectRatio: false, scales: { y: { grid: { color: '#333'} }, x: { grid: { color: '#333'} } } } });";

  // FUNÇÃO DE ATUALIZAÇÃO (AGORA COM LÓGICA DE CONEXÃO)
  p += "function updateData() {";
  p += "  fetch('/data')";
  p += "    .then(r => {";
            // SE A RESPOSTA CHEGOU, ESTAMOS CONECTADOS
  p += "       document.getElementById('led').className = 'led led-on';";
  p += "       document.getElementById('statusText').innerHTML = 'Conectado';";
  p += "       document.getElementById('statusText').style.color = '#fff';";
  p += "       return r.json();";
  p += "    })";
  p += "    .then(d => {";
  p += "      document.getElementById('temp1').innerHTML = parseFloat(d.max6675).toFixed(1) + ' °C';";
  p += "      document.getElementById('temp2').innerHTML = parseFloat(d.ntc).toFixed(1) + ' °C';";
  p += "      var now = new Date().toLocaleTimeString();";
  p += "      chart.data.labels.push(now);";
  p += "      chart.data.datasets[0].data.push(d.max6675);";
  p += "      chart.data.datasets[1].data.push(d.ntc);";
  p += "      if(chart.data.labels.length > 60) {";
  p += "        chart.data.labels.shift(); chart.data.datasets[0].data.shift(); chart.data.datasets[1].data.shift();";
  p += "      }";
  p += "      chart.update();";
  p += "    })";
  p += "    .catch(e => {";
            // SE DEU ERRO (CATCH), ESTAMOS DESCONECTADOS
  p += "       document.getElementById('led').className = 'led led-off';";
  p += "       document.getElementById('statusText').innerHTML = 'Desconectado';";
  p += "       document.getElementById('statusText').style.color = '#ff4444';";
  p += "    });";
  p += "}";
  
  p += "setInterval(updateData, 1000);";
  p += "</script></body></html>";
  
  return p;
}

// ---------------------------
// ENDPOINTS
// ---------------------------
void handleRoot() {
  server.send(200, "text/html", getWebPage());
}

void handleData() {
  float tMax = thermocouple.readCelsius();
  float tNTC = readNTCTemp();
  if (isnan(tMax)) tMax = 0.0;
  if (isnan(tNTC)) tNTC = 0.0;

  String json = "{";
  json += "\"max6675\": " + String(tMax, 2) + ",";
  json += "\"ntc\": " + String(tNTC, 2);
  json += "}";
  server.send(200, "application/json", json);
}

// ---------------------------
// SETUP
// ---------------------------
void setup() {
  Serial.begin(115200);
  analogReadResolution(12); 

  Serial.print("Conectando WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) { 
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado! IP: ");
    Serial.println(WiFi.localIP());
    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.begin();
  } else {
    Serial.println("\nFalha ao conectar no WiFi. Reiniciando em 3s...");
    delay(3000);
    ESP.restart(); 
  }
}

void loop() {
  server.handleClient();
  delay(2);
}