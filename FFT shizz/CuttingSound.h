#ifndef _CUTTING_SOUND_H
#define _CUTTING_SOUND_H

#include <arduinoFFT.h>

// Some setup for output print FFT thingy
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

typedef struct {

	double freq;
	double magnitude;

} SpectralSample;

class CuttingSound {

private:
    // Setup constants
    static const uint16_t buffer_size = 64; // MUST be power of two
    static const double sampling_frequency = 10000; // Sampling frequensy in Hz
    static const int sampling_time = 100; // Sample time in microseconds
    static const int microphone_pin = A0;

    arduinoFFT FFT;

	public:
		CuttingSound();
		void getSpectrum();
		double getMajorPeak();
		SpectralSample spectrum[buffer_size];

	private:
  
		// Sample buffers
		double vReal[buffer_size];
		double vImag[buffer_size];

		// get sound sample to calculate fft of
		void sampleSound();
	
		void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);


};

#endif
