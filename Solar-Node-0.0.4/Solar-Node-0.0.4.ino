//

/** 

The MIT License (MIT) etc...
See more at https://thingpulse.com
*/

// --------- Intro / firmware ID block ----------
#define FW_FILE   __FILE__          // auto filename
#define FW_VER    "0.0.4"          // update per release

void intro() {//Let's assume this is the latest Intro code and call it V0.0.1
  Serial.println(F("\n===== FIRMWARE INFO ====="));
  Serial.printf("Function: %s\n", __func__);
  Serial.printf("File: %s\n", FW_FILE);
  Serial.printf("Version: %s\n", FW_VER);
  Serial.printf("Binary MD5: %s\n", ESP.getSketchMD5().c_str()); // runtime binary hash
  Serial.printf("Compiled: %s %s\n", __DATE__, __TIME__);
  Serial.printf("Flash Size: %u bytes\n", ESP.getFlashChipRealSize());
  Serial.printf("SDK: %s\n", ESP.getSdkVersion());
  Serial.printf("Current Hardware Setup: SDA = ESP8266 NODEMCU PIN D2, SDA = ESP8266 NODEMCU PIN D1 = SCL");
  Serial.println(F("=========================\n"));
}
// call intro() early in setup()

#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "Adafruit_INA219.h"

// Display Settings for OLED
const int I2C_DISPLAY_ADDRESS = 0x3c;
#if defined(ESP8266) //It is when using nodemcu
const int SDA_PIN = D2;
const int SCL_PIN = D1;
#else
const int SDA_PIN = D3; //D3; 
const int SCL_PIN = D4; //D4;
#endif

 // Initialize the oled display for address 0x3c
 // sda-pin=14 and sdc-pin=12
 SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SCL_PIN);
 OLEDDisplayUi   ui( &display );

Adafruit_INA219 ina219;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  intro();
 
  // initialize display
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


  Serial.print("Bus Voltage"); Serial.print("\t"); // Bus Voltage
  Serial.print("Shunt Voltage"); Serial.print("\t"); // Shunt Voltage
  Serial.print("Load Voltage"); Serial.print("\t"); // Load Voltage
  Serial.print("Current"); Serial.print("\t");  // Current
  Serial.println("Power");  // Power
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
  display.drawString(32, 14, "shuntvoltage:");
  display.drawString(30, 24, "loadvoltage:");
  display.drawString(30, 34, "current_mA:");
  display.drawString(30, 43, "power_mW:");
  display.drawString(35, 53, "Uptime in sec:");
  display.drawString(80, 4, String(busvoltage));
  display.drawString(80, 15, String(shuntvoltage));
  display.drawString(80, 25, String(loadvoltage));
  display.drawString(80, 35, String(current_mA));
  display.drawString(80, 44, String(power_mW));
  display.drawString(80, 53, String(millis()/1000));

  display.display();
  delay(1000);
}