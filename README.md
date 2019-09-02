# Photovoltaic Logger

PV_logger ist ein ESP8266 Arduino Projekt zur Remotekontrolle von PV-Anlagen.
Das Projekt ist bisher ein privates GitHub repository und lediglich ein Proof-of-Concept für kostengünstige Überwachung von 
PV-Anlagen.
# Feature

  - Messung von bis zu vier Spannungsquellen von 0-48 Volt(Batterien der PV-Anlage)
  - Messung von Temperatur und Luftfeuchte um die Batterien herum
  - Datenübermittlung per WiFi und MQTT
  - Visualiesierung via [Cayenne](https://mydevices.com/) (Account wird benötigt!)

# Geplante Feature

  - LoRa kompatibilität für erhöhte Reichweite zur Datenübertragung
  - Messung der Panelleistung
  - Messung des Energieverbrauchs
  - Datenaufnahme über NODERED
  - Datenvisualisierung mit hilfe von Grafana und InfluxDB
# Technik
### Verwendete Hardware
- NodeMCU Board mit ESP8266
- BME280 für Temperatur und Luftfeuchte
- 4 Channel ADS1015 12-Bit ADC auf Breakoutboard von Adafruit
- Widerstände für passende Spannungsteiler
### Software
- Arduino Bibliothek für den ESP8266 
- SparkFunBME280 Bibliothek für den BME280 Sensor
- CayenneMQTTESP8266 Bibliothek für die Kommunikation mit Cayenne via MQTT
- SSD1306 für den Display
- Adafruit_ADS1015 Bibliothek für den 4 Channel ADC
# Installation der nötigen Software
Ich habe zum Aufspielen des Sourcecodes Atom mit dem [PlatformIO Plug-In](https://platformio.org/) verwendet. Eine ausführliche Doukmentation dazu findet sich [hier](https://docs.platformio.org/en/latest/).
Die benötigten Bibliotheken lassen sich direkt in Atom installieren. Auch hierzu gibt es eine [Anleitung](https://docs.platformio.org/en/latest/librarymanager/index.html).
# Funktionsprinzip
Ein 10/1 Spannungsteiler wird zur direkten Spannungsmessung der Batterien verwendet. Im code wird mit dem ADC die Analoge Spannung digitalisiert und wieder in eine __*float*__ umgerechnet um dann mit dem Teilerverhältnis in die gemesse Spannung umgewandelt zu werden. Anschließend  wird die gemessene Spannung zusammen mit der Temperatur und der Luftfeuchte per MQTT an __*Cayenne*__ geschickt. Für Testzwecke geschiet das noch alle 20 Sekunden. Später wird das nicht nötig sein, weil die zu messenden Batterien zu träge sind und ihre Spannung nicht blitzschnell ändern werden.


