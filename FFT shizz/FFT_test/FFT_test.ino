#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT();


const uint16_t buffer_size = 64; // MUST be power of two
const double sampling_frequency = 10000;

// Sample buffers
double vReal[buffer_size];
double vImag[buffer_size];

// Some setup for FFT thingy
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Wollah G");
}

void loop() {

  // Do fft shizzz
  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */

    
}
