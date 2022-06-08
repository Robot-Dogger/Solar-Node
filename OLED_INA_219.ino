
/**The MIT License (MIT) etc...
See more at https://thingpulse.com
*/

#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "Adafruit_INA219.h"

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
#if defined(ESP8266)
const int SDA_PIN = D2;
const int SDC_PIN = D1;
#else
const int SDA_PIN = 5; //D3; 
const int SDC_PIN = 4; //D4;
#endif

 // Initialize the oled display for address 0x3c
 // sda-pin=14 and sdc-pin=12
 SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
 OLEDDisplayUi   ui( &display );

Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
 
  // initialize dispaly
  display.init();
  display.clear();
  display.display();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);
  delay(500);
  Serial.print(".");
  display.clear();
  display.drawString(64, 4, "Screen Is Working!");
  display.display();
  Serial.println("");



  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }


  Serial.print("BV"); Serial.print("\t"); // Bus Voltage
  Serial.print("SV"); Serial.print("\t"); // Shunt Voltage
  Serial.print("LV"); Serial.print("\t"); // Load Voltage
  Serial.print("C"); Serial.print("\t");  // Current
  Serial.println("P");  // Power
  //display.clear();
  //display.drawString(64, 10, "Screens working!");

}



void loop() {
  
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  Serial.print(busvoltage); Serial.print("\t"); 
  Serial.print(shuntvoltage); Serial.print("\t");
  Serial.print(loadvoltage); Serial.print("\t");
  Serial.print(current_mA); Serial.print("\t");
  Serial.println(power_mW);

  delay(1000);

  display.clear();
  display.drawString(30, 4, "busvoltage:");
  display.drawString(30, 15, "shuntvoltage:");
  display.drawString(30, 25, "loadvoltage:");
  display.drawString(30, 35, "current_mA:");
  display.drawString(30, 45, "power_mW:");
  display.drawString(80, 4, String(busvoltage));
  display.drawString(80, 15, String(shuntvoltage));
  display.drawString(80, 25, String(loadvoltage));
  display.drawString(80, 35, String(current_mA));
  display.drawString(80, 45, String(power_mW));

  display.display();
  delay(1000);
}
