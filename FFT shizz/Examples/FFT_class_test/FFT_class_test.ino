#include "CuttingSound.h" // Place the .h and .cpp file in your working directory

    // Setup constants
    static const uint16_t buffer_size = 64; // MUST be power of two
    static const double sampling_frequency = 10000; // Sampling frequensy in Hz
    static const int sampling_time = 100; // Sample time in microseconds
    static const int microphone_pin = A0;

CuttingSound cutting_sound = CuttingSound(buffer_size, sampling_frequency, sampling_time, microphone_pin);

SpectralSample * spectrum = malloc(buffer_size * sizeof(SpectralSample));

void setup() {

  Serial.begin(115200);
  Serial.println("Wollah G!");

}

void loop() {

  spectrum = cutting_sound.getSpectrum();

  for (int i = 0; i < 64; i++){

    Serial.print(spectrum[i].freq);
    Serial.print(" Hz : ");
    Serial.print(spectrum[i].magnitude);
    Serial.print("\n");
    
  }

  
  
  Serial.println("Major Peak @");
  Serial.println(cutting_sound.getMajorPeak());

}
