#include <arduinoFFT.h>

arduinoFFT FFT = arduinoFFT();


const uint16_t buffer_size = 64; // MUST be power of two
const double sampling_frequency = 10000;
const int sampling_time = 100; // sampling time in MICROSECONDS
const int microphone_pin = A0;

// Sample buffers
double vReal[buffer_size];
double vImag[buffer_size];

// Some setup for output print FFT thingy
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

  // Do samplings
  for (uint16_t i = 0; i < buffer_size; i++){

    // Sleep for the required sample time (This may or may not be accurate)
    delayMicroseconds(sampling_time);
    // Do analog read
    vReal[i] = analogRead(microphone_pin);
    vImag[i] = 0.0;
    //Serial.println(vReal[i]);
  }
  

  // Do fft shizzz
  FFT.Windowing(vReal, buffer_size, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, buffer_size, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, buffer_size); /* Compute magnitudes */

  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (buffer_size >> 1), SCL_FREQUENCY);

    
}


// Totally not stolen from the example...
void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / sampling_frequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * sampling_frequency) / buffer_size);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
