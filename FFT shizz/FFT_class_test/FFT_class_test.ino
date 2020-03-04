#include "CuttingSound.h" // Place the .h and .cpp file in your working directory


CuttingSound cutting_sound = CuttingSound();

void setup() {

  Serial.begin(115200);
  Serial.println("Wollah G!");


}

void loop() {

  cutting_sound.getSpectrum();
  Serial.println("Major Peak @");
  Serial.println(cutting_sound.getMajorPeak());

}
