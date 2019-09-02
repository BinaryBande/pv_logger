#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>
#include "ads.h"
#include "secrets.h"
#include <SparkFunBME280.h>
#include <CayenneMQTTESP8266.h>


#include "SSD1306.h"
#define CAYENNE_PRINT Serial
#define CAYENNE _DEBUG
//Cayenne
char username[] = "";
char password[] = "";
char clientID[] = "";



//WLAN
char ssid[]=SECRET_SSID;
char passwort[]=SECRET_PASS;
WiFiClientSecure client;

//Analog-DigitalConverter
ads ads;
int16_t adc0, adc1, adc2, adc3;
//Display
SSD1306 display (0x3c, D2,D1);

//Umweltsensor
BME280 bme;
float humi, temp;


float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0;
float R2 = 10000.0;
int values = 0;
unsigned long lastMillis = 0;
int i=0,datensatz=1;
float calcvout(int val, int channel)
{

  val = ads.readADC_SingleEnded(channel);
  float v = ((val/2^11)*4.096)/1000.0;
  return v;
}

bool connecToWifi()
{
  int resetTicker = 0;            // Reset Ticker, damit ESP sich selbst resetten kann
  int tickDelay = 500;
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // WLan-Verbindung aufbauen
  Serial.println();
  Serial.println();
  Serial.print("Verbinde mit ");
  Serial.println(ssid);
  WiFi.begin(ssid,passwort); //Versucht sich ins WLan anzumelden
  while(WiFi.status() != WL_CONNECTED) { //solange keine WLan-Verbindung besteht
    delay(tickDelay);
    Serial.print(".");              //Alle 500ms einen Punkt ausgeben
    resetTicker++;                  //Alle 500ms Ticker um 1 erhöhen
    if(resetTicker >= 25) {         //Bei 25 versuchen neustarten
      Serial.println(".");
      Serial.println("Verbindung konnte nicht aufgebaut werden");
      delay(3000);
      ESP.restart();               //ESP neustarten
    }
  }
  Serial.println(" ");
  Serial.println("WLan verbunden");
  Serial.println("die IP Adresse lautet: ");
  Serial.println(WiFi.localIP()); //Bei Wlan_Verbindung IP adresse ausgeben
  return true;
}

void displaybegin()
{
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
}



void setup() {
  displaybegin();
  display.drawString(0, 0, "Verbinde");
  display.drawString(0, 16, "mit WLAN");
  display.display();
  Serial.begin(9600);

  if (connecToWifi())
      {
        display.clear();
        display.drawString(0, 0, "Mit WLAN");
        display.drawString(0, 16, "verbunden");
        display.display();
        delay(4000);
      }

//connecToWifi();
  Cayenne.begin(username, password, clientID, ssid, passwort);
  Wire.begin(D2,D1);   //D2= SDA ,D1=SCL
  ads.begin();
  ads.setGain(GAIN_ONE);
  bme.beginI2C();

  }





void loop() {
  Cayenne.loop();
  if (millis() - lastMillis > 20000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
    Cayenne.virtualWrite(0, datensatz);
    Cayenne.virtualWrite(1, bme.readTempC(), "temp", "c");
    Cayenne.virtualWrite(2,bme.readFloatHumidity(),"RH","%");
    Cayenne.virtualWrite(3,calcvout(adc2,2),"Spannung","V");
    datensatz++;
    Serial.println(temp);
    Serial.println(humi);
    Serial.println(vout);
  }
  vout=calcvout(adc2,2);
  humi=bme.readFloatHumidity();
  temp=bme.readTempC();
  display.clear();
  display.drawString(0, 0, "Spannung: " + String(vout) + "V\t");
  display.drawString(0, 16, "IP:" +WiFi.localIP().toString());
  display.drawString(0, 32, "Temp:" +String(temp)+ "C\t");
  display.drawString(0, 48, "RH:" + String(humi)+ "%\t");
  display.display();


  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("kein wlan");
  }
  else{
    Serial.println(i);
    i++;

  }

}
