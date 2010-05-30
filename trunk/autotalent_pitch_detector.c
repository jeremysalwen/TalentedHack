#include "autotalent_pitch_detector.h"
const float * obtain_autocovariance(PitchDetector *pdetector, fft_vars* fftvars, CircularBuffer* buffer,long int N) {
	// Window and fill FFT buffer
	long int i;
	for (i=0; i<N; i++) {
		float windowval=pdetector->cbwindow[i];
		
		float inputbuffer=buffer->cbi[(buffer->cbiwr-i+N)%N];
		fftvars->ffttime[i] = inputbuffer*windowval;
		//pl_fpoint(i,fftvars->ffttime[i]);
	}
	// Calculate FFT
	fft_forward(fftvars);

	// Remove DC
	fftvars->complex[0][0] = 0;
	fftvars->complex[0][1] = 0;
    long int Nf=N/2;
	// Take magnitude squared
	for (i=1; i<Nf; i++) {
		fftvars->complex[i][0] = (fftvars->complex[i][0] )*(fftvars->complex[i][0]) + (fftvars->complex[i][1])*(fftvars->complex[i][1]);
		fftvars->complex[i][1] = 0;
	}

	// Calculate IFFT
	fft_inverse(fftvars);

	// Normalize
	float tf = (float)1/(fftvars->ffttime[0]); //Everything is divided by N because fftw doesn't normalize, and instead introduces a factor of N
	for (i=1; i<N; i++) {
		fftvars->ffttime[i] = fftvars->ffttime[i] * tf;
	}
	fftvars->ffttime[0] = 1;
	
	return fftvars->ffttime;
}

float get_pitch_period(PitchDetector * pdetector, const float* autocorr, unsigned long Nf, float fs) {
	// Calculate pitch period
	//   Pitch period is determined by the location of the max (biased)
	//     peak within a given range
	
	//   Confidence is determined by the corresponding unbiased height

	float pperiod = pdetector->pmin;
	const float* i;
	const float* start=autocorr+pdetector->nmin;
	const float* end=autocorr+pdetector->nmax;
	const float* nextmax=autocorr+Nf;
	if(end>=nextmax) {
		end=nextmax-1;
	}
	const float* peak=start;
	for (i=start; i<end; i++) {
		const float* previous = i-1;
		const float* next = i+1;
		if (*i>*previous && *i>=*next && *i>*peak) {
			peak = i;
		}
	}
	if (peak>start) {
		int peakindex=peak-autocorr;
		pdetector->confidence = (*peak) * pdetector->acwinv[peakindex];
		if (peak<end) {
			//Parabolically interpolate to find the peak
			int denominator=2*peak[0]-peak[1]-peak[-1];
			if(denominator!=0) {
				int numerator=peak[1]-peak[-1];
				float fmax=peakindex+((float)numerator)/((float)denominator);
				pperiod=fmax/fs;
			} else {
				pperiod=peakindex/fs;
			}
			/*
			// Find the center of mass in the vicinity of the detected peak
			float tf = peak[-1]*(peakindex-1);
			tf = tf + peak[0]*(peakindex);
			tf = tf + peak[1]*(peakindex+1);
			tf = tf/(peak[-1] + peak[0] + peak[1]);
			pperiod = tf/fs;
			*/
		} else {
			pperiod = (float)(peakindex)/fs;
		}
	} else {
		pdetector->confidence=0;
	}
	
	// Convert to semitones
	if (pdetector->confidence>=pdetector->vthresh) {

		return pperiod;
	} else {
		return -1;  //Could not find pitch;
	}
}

void InstantiatePitchDetector(PitchDetector * pdetector,fft_vars* fftvars, unsigned long cbsize, double SampleRate) {
    unsigned long corrsize=cbsize/2+1;
	pdetector->pmax = 1/(float)70;  // max and min periods (ms)
	pdetector->pmin = 1/(float)700; // eventually may want to bring these out as sliders

	pdetector->nmax = (unsigned long)(SampleRate * pdetector->pmax);
	if (pdetector->nmax > corrsize) {
		pdetector->nmax =corrsize;
	}
	pdetector->nmin = (unsigned long)(SampleRate * pdetector->pmin);
	pdetector->vthresh = 0.7;  //  The voiced confidence (unbiased peak) threshold level
			// Generate a window with a single raised cosine from N/4 to 3N/4
	pdetector->cbwindow=(float*)calloc(cbsize, sizeof(float));
	int i;
	for (i=0; i<(cbsize/2 ); i++) {
		pdetector->cbwindow[i+cbsize/4] = -0.5*cos(4*PI*i/(cbsize - 1)) + 0.5;
	}
	// ---- Calculate autocorrelation of window ----
	pdetector->acwinv = calloc(cbsize, sizeof(float));
	memcpy(fftvars->ffttime,pdetector->cbwindow,cbsize*sizeof(float));     
	fft_forward(fftvars);
	
	for (i=0; i<corrsize; i++) {
		fftvars->complex[i][0] = (fftvars->complex[i][0] )*(fftvars->complex[i][0]) + (fftvars->complex[i][1])*(fftvars->complex[i][1]);
		fftvars->complex[i][1] = 0;
	}
	fft_inverse(fftvars);
	for (i=1; i<cbsize; i++) {
		pdetector->acwinv[i] = fftvars->ffttime[i]/fftvars->ffttime[0];
		if (pdetector->acwinv[i] > 0.000001) {
			pdetector->acwinv[i] = (float)1/pdetector->acwinv[i];
		}
		else {
			pdetector->acwinv[i] = 0;
		}
	}
	pdetector->acwinv[0] = 1;
	// ---- END Calculate autocorrelation of window ----
	
}