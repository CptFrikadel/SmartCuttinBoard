#include "CuttingSound.h"

CuttingSound::CuttingSound(const uint16_t _buffer_size, 
				const double _sampling_freq, 
				const int _sampling_time,
				const int _microphone_pin) :
	buffer_size(_buffer_size),
	sampling_frequency(_sampling_freq),
	sampling_time(_sampling_time),
	microphone_pin(_microphone_pin) {


	}

void CuttingSound::getSpectrum(){

  // Do fft shizzz
  FFT.Windowing(vReal, buffer_size, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, buffer_size, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, buffer_size); /* Compute magnitudes */

  Serial.println("Computed magnitudes:");
  PrintVector(vReal, (buffer_size >> 1), SCL_FREQUENCY);

}

void CuttingSound::sampleSound(){

	// Do samplings
	for (uint16_t i = 0; i < buffer_size; i++){

		// Sleep for the required sample time (This may or may not be accurate)
		delayMicroseconds(sampling_time); // This is a blocking call
		// Do analog read
		vReal[i] = analogRead(microphone_pin);
		vImag[i] = 0.0;
  }
}


double CuttingSound::getMajorPeak(){

	return FFT.MajorPeak(vReal, buffer_size, sampling_frequency);
}
