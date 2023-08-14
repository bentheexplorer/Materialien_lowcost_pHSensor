/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID   ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN    ""


#include "DFRobot_ESP_PH.h"
#include <EEPROM.h>
DFRobot_ESP_PH ph;
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

#define ESPADC 4096.0   //ESP Analog Digital Konvertierungswert
#define ESPVOLTAGE 3300 //ESP Spannungsversorgung
#define PH_PIN 35    //ESP Pin Nummer
float voltage, phValue, temperature = 25;

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V4)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup()
{
  //Debug console
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  EEPROM.begin(32);//Speichererlaubnis der Kalbibrierung
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
    Blynk.virtualWrite(V4, phValue);

    Serial.print("Spannung: ");
    Serial.print(voltage, 2);
    Serial.println(" mV");

    // Serial.print("Temperatur:");
    // Serial.print(temperature, 1);
    // Serial.println(" °C");
    
    Serial.print("pH: ");
    Serial.println(phValue, 1);
    Serial.println();
    
    delay(5000);
 }
  ph.calibration(voltage, temperature); // Kalibrierung via Seriellen Monitor
        }
