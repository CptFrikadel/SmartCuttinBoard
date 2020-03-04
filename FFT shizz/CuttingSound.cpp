#include "CuttingSound.h"

CuttingSound::CuttingSound(){

  FFT = arduinoFFT();
}

void CuttingSound::getSpectrum(){

  sampleSound();

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

void CuttingSound::PrintVector(double *vData, uint16_t bufferSize, uint8_t scaleType)
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
