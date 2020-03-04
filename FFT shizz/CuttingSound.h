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

} SpectalSample;

class CuttingSound {

	public:
		CuttingSound(const uint16_t _buffer_size, 
				const double _sampling_freq, 
				const int _sampling_time,
				const int _microphone_pin);

		void getSpectrum();
		double getMajorPeak();
		SpectralSample spectrum[buffer_size];

	private:
		
		// Setup constants
		const uint16_t buffer_size = 64; // MUST be power of two
		const double sampling_frequency = 10000; // Sampling frequensy in Hz
		const int sampling_time = 100; // Sample time in microseconds
		const int microphone_pin = A0;

		// Sample buffers
		double vReal[buffer_size];
		double vImag[buffer_size];

		// get sound sample to calculate fft of
		void sampleSound();


};

#endif
