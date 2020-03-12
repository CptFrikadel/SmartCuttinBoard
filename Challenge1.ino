/* 
 This code contains (parts of) example code from:
 
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

&

Example code from the LiquidCrystal_I2C library

&

Example code from the OOCSI-ESP library

*/

#include "HX711.h"
#include <LiquidCrystal_I2C.h>
//#include "OOCSI.h"

#define calibration_factor -300 //This value is obtained using the calibration sketch

#define DOUT  3
#define CLK  2

// SSID of your Wifi network
//const char* ssid = "yourssid";
// Password of your Wifi network.
//const char* password = "yourpassword";

// name for connecting with OOCSI (unique handle)
//const char* OOCSIName = "cutboard";
// put the adress of your OOCSI server here, can be URL or IP address string
//const char* hostserver = "oocsi.id.tue.nl";

HX711 scale;
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

lcd.init();
lcd.backlight();

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");

//  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);
}

void loop() {
  lcd.clear();
  
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" grams"); 
  Serial.println();

  lcd.setCursor(4,0);
  lcd.print("Weight:");
  lcd.setCursor(5,1);
  lcd.print(scale.get_units(),1);
  lcd.print("g");

  //oocsi.newMessage("boardWeightChannel");
  //oocsi.addFloat("weight", scale.get_units());
  //oocsi.sendMessage();
  //oocsi.check();

  delay(500);
}
