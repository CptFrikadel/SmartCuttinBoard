#include "CuttingSound.h"


CuttingSound cutting_sound = CuttingSound();

void setup() {

  Serial.begin(115200);
  Serial.println("Wollah G!");


}

void loop() {
  // put your main code here, to run repeatedly:

  cutting_sound.getSpectrum();

}
