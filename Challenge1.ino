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

  &

  Inspiration from code by @trobs3 on the Arduino Forum.
*/

#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include "OOCSI.h"
#include "CuttingSound.h"
#include <Wire.h>
#include <TimerOne.h>

#define calibration_factor -300 //This value is obtained using the calibration sketch

#define DOUT  3
#define CLK  2

#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C

#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

boolean timingFlag = true;
unsigned long cuttingFreq;
unsigned long timingMillis;

// SSID of your Wifi network
const char* ssid = "yourssid";
// Password of your Wifi network.
const char* password = "yourpassword";

// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "cutboard";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

// Setup constants
static const uint16_t buffer_size = 64; // MUST be power of two
static const double sampling_frequency = 10000; // Sampling frequensy in Hz
static const int sampling_time = 100; // Sample time in microseconds
static const int microphone_pin = A0;

CuttingSound cutting_sound = CuttingSound(buffer_size, sampling_frequency, sampling_time, microphone_pin);  //set up a new instance of cutting_sound
SpectralSample * spectrum = malloc(buffer_size * sizeof(SpectralSample));   //Set up a memory block for the spectrum


HX711 scale;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// This function read Nbytes bytes from I2C device at address Address.
// Put read bytes starting at register Register in the Data array.
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();

  // Read Nbytes
  Wire.requestFrom(Address, Nbytes);
  uint8_t index = 0;
  while (Wire.available())
    Data[index++] = Wire.read();
}

// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

// Initial time
long int ti;
volatile bool intFlag = false;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  I2CwriteByte(MPU9250_ADDRESS, 29, 0x06);    // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_4_G);    // Configure accelerometers range

  lcd.init();
  lcd.backlight();

  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
  pinMode(12, OUTPUT);
  Timer1.initialize(10000); // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback); // attaches callback() as a timer overflow interrupt

  ti = millis();  // Store initial time
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);
}

// Counter
long int cpt = 0;

void callback()
{
  intFlag = true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

void loop() {
  lcd.clear();

  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" grams");
  Serial.println();

  lcd.setCursor(4, 0);
  lcd.print("Weight:");
  lcd.setCursor(5, 1);
  lcd.print(scale.get_units(), 1);
  lcd.print("g");

  oocsi.newMessage("boardWeightChannel");
  oocsi.addFloat("weight", scale.get_units());
  oocsi.sendMessage();
  oocsi.check();

  spectrum = cutting_sound.getSpectrum();

  for (int i = 0; i < 64; i++) {

    Serial.print(spectrum[i].freq);
    Serial.print(" Hz : ");
    Serial.print(spectrum[i].magnitude);
    Serial.print("\n");
  }
  while (!intFlag);
  intFlag = false;

  // Read accelerometer
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS, 0x3B, 14, Buf);

  // Create 16 bits values from 8 bits data

  // Accelerometer
  int16_t ax = -(Buf[0] << 8 | Buf[1]);
  int16_t ay = -(Buf[2] << 8 | Buf[3]);
  int16_t az = Buf[4] << 8 | Buf[5];

  // Map accelerometer values to smooth it out
  int xx = map(ax, -4000, 4000, 0, 10);

  // Display values
  Serial.println (xx, DEC);
  delay(50);

  if (timingFlag == false && xx < 7)
  {
    timingFlag = true;
    timingMillis = millis();
  }

  if (timingFlag == true && xx >= 7)
  {
    cuttingFreq = millis() - timingMillis;
    timingFlag = false;

    oocsi.newMessage("cuttingSpeedChannel");
    oocsi.addFloat("frequency", int xx);
    oocsi.sendMessage();
    oocsi.check();

    delay(500);
  }
