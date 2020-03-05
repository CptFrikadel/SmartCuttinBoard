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
    const uint16_t buffer_size = 64; // MUST be power of two
    const double sampling_frequency = 10000; // Sampling frequensy in Hz
    const int sampling_time = 100; // Sample time in microseconds
    const int microphone_pin = A0;

    arduinoFFT FFT;

	public:
		CuttingSound(const uint16_t buffer_size,
		              const double sampling_frequency,
		              const int sampling_time,
		              const int microphone_pin);
		~CuttingSound();
		SpectralSample * getSpectrum();
		void calculateSpectrum();
		double getMajorPeak();
		SpectralSample * spectrum;

	private:
  
		// Sample buffers
		double * vReal;
		double * vImag;

		// get sound sample to calculate fft of
		void sampleSound();
	
		void PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType);


};

#endif
