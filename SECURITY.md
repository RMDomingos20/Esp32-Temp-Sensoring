# Política de Segurança

## Versões Suportadas

Atualmente, este projeto é um protótipo acadêmico e não possui um ciclo de lançamento de versões regulares. A versão presente no branch `main` é a única suportada.

## Relatando Vulnerabilidades

Como este é um projeto desenvolvido para fins didáticos na disciplina de Instrumentação, não há uma equipe dedicada de resposta a incidentes. No entanto, se você encontrar falhas críticas, sinta-se à vontade para abrir uma **Issue** neste repositório.

## Notas de Segurança Importantes

Se você pretende utilizar este código ou hardware, esteja ciente dos seguintes pontos:

### 1. Credenciais WiFi Hardcoded
O código fonte (`.ino`) contém variáveis para `ssid` e `password` do WiFi.
* **Risco:** Se você fizer um commit dessas credenciais em um repositório público, terceiros poderão ver sua senha de rede.
* **Recomendação:** Nunca suba o código com suas senhas reais. Use um arquivo de configuração separado ou variáveis de ambiente se for expandir o projeto.

### 2. Protocolo HTTP (Sem Criptografia)
O servidor web do ESP32 roda sobre HTTP na porta 80.
* **Risco:** Os dados trafegam em texto plano pela rede local. Não há criptografia (HTTPS/TLS).
* **Recomendação:** Não utilize este sistema para transmitir dados sensíveis ou em redes públicas não confiáveis.

### 3. Isolamento Elétrico
* O projeto lida com sensores de temperatura. Certifique-se de que o Termopar e o NTC estejam devidamente isolados eletricamente se forem submersos ou fixados em superfícies condutoras, para evitar danos ao ESP32 ou leituras erradas no ADC.
