#include "DFRobot_ESP_PH.h"
#include <EEPROM.h>
DFRobot_ESP_PH ph;
#define BLYNK_USE_DIRECT_CONNECT
#include <BlynkSimpleEsp32_BLE.h>
#include <BLEDevice.h>
#include <BLEServer.h>
//#define BLYNK_PRINT Serial
char auth[] = "QB4zgLvHJgqhs-RMGBh5v2kDXnO8cFC4";
#define ESPADC 4096.0   //ESP Analog Digital Konvertierungswert
#define ESPVOLTAGE 3300 //ESP Spannungsversorgung
#define PH_PIN 35    //ESP Pin Nummer
float voltage, phValue, temperature = 25;

void setup()
{
  //Debug console
  Serial.begin(9600);
  Serial.println("Verbindungsaufbau...");
  Blynk.setDeviceName("Blynk");
  Blynk.begin(auth);
  EEPROM.begin(32);//Speichererlaubnis der Kalibrierung
  ph.begin();
}

void loop()
{
  Blynk.run();
   static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) //Zeitintervall: 1s
  {
    timepoint = millis();
    voltage = analogRead(PH_PIN) / ESPADC * ESPVOLTAGE; // Ausgabe der Spannung
    phValue = ph.readPH(voltage, temperature); // Berechnung des pH-Werts anhand 
    //der Spannung mit Temperaturkompensation (insofern möglich)
    Blynk.virtualWrite(V1, phValue);

    Serial.print("Spannung:");
    Serial.print(voltage, 2);
    Serial.println(" mV");

//    Serial.print("Temperatur:");
//    Serial.print(temperature, 1);
//    Serial.println(" °C");
    
    Serial.print("pH:");
    Serial.println(phValue, 1);
    Serial.println();
    
    delay(2000);
 }
  ph.calibration(voltage, temperature); // Kalibrierung via Seriellen Monitor
        }
