#include "pitch_detector.h"
#include "lv2.h"

#ifdef DEBUGPLOT
#include <time.h>


void setpixel8(SDL_Surface *screen, int x, int y,Uint32 colour )
{
	Uint8 *pixmem8 = (Uint8*) screen->pixels  + y*1024 + x;
	*pixmem8 = colour;
}
#endif
const float * obtain_autocovariance(PitchDetector *pdetector, fft_vars* fftvars, CircularBuffer* buffer,long int N) {
#ifdef DEBUGPLOT
	if(!printed) {
		SDL_FillRect( screen, NULL, 0 );

	}

	Uint32 colour= SDL_MapRGB( screen->format, 255,255,255);
#endif
	// Window and fill FFT buffer
	long int i;
	for (i=0; i<N; i++) {
		float windowval=pdetector->cbwindow[i];
		float inputbuffer=buffer->cbi[(buffer->cbiwr-i+N)%N];
		fftvars->ffttime[i] = inputbuffer*windowval;
#ifdef DEBUGPLOT
		if(i&1 && !printed) {
			setpixel8(screen,i/2,fftvars->ffttime[i]*-50+50,colour);
		}
#endif
	}
#ifdef DEBUGPLOT
	if(!printed) {
		printed=1;
	}
#endif
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
	const float noise_floor = 0.05;  // To prevent rounding errors when quiet.
	float tf = (float)1/fmax(fftvars->ffttime[0], noise_floor);
	for (i=1; i<N; i++) {
		// Mathematically x[i] <= x[0], but there can be rounding errors.
		fftvars->ffttime[i] = fmin(fftvars->ffttime[i] * tf, 1.0);
	}

	fftvars->ffttime[0] = 1;
	return fftvars->ffttime;
}

float get_pitch_period(PitchDetector * pdetector, const float* autocorr, unsigned long Nf, float fs) {
	// Calculate pitch period

	//MPM Algorithm, thanks to Philip McLeod, and Geoff Wyvill, adapted from their GPL Tartini program

	float pperiod = pdetector->pmin;
	const float* end=autocorr+pdetector->nmax;
	const float* start=autocorr+pdetector->nmin;

	//circular buffer of peaks.
	const int numpeaks=2000;
	const float* peaks[numpeaks];
	memset(peaks,0, sizeof(float*) * numpeaks);
	//current write pointer
	const float** peakindex=peaks;
	//end of buffer
	const float** endpeak=peaks+numpeaks;
	//Choose peak so far
	const float** bestpeakindex=peaks;

	//This tells us if the interval we are testing has a peak higher than all previous peaks.  If not, we can ignore it, because it will never be picked.
	bool newmaxima=false;

	const float* i=autocorr;
	//go to second zero-crossing
	while(*i>=0 && i<end) i++;
	while(*i<=0 && i<end) i++;
	while(i<start) i++;
	*peaks=i;
	float peak=-2; //height of highest peak found
	bool abovezero=true;  //Was the last sample above zero?

	while (i<end) {
		if(*i<=0) {
			//If the height of this last peak was bigger than all before it.
			if(abovezero && newmaxima) {
				//recalculate best peak index;
				while(**bestpeakindex < *pdetector->p_ppickthresh * peak) {
					bestpeakindex++;
					if(bestpeakindex>=endpeak) {
						bestpeakindex=peaks;
					}
				}
				peakindex++;
				if(peakindex>=endpeak) {
					peakindex=peaks;
				}
				if(peakindex==bestpeakindex) {
					//Our circular buffer wrapped around! This is bad!  Should I throw an error or just return the best one we have so far?
					fprintf(stderr,"TalentedHack Error! Peak picking buffer wrapped around! Very bad!\n");
					break;
				}
				*peakindex=i;
				newmaxima=false;
			}
			abovezero=false;
		} else {
			if (*i>peak) {
				peak = *i;
				newmaxima=true;
			}
			if(*i>**peakindex) {
				*peakindex=i;
			}
			abovezero=true;
		}
		i++;		
	}

	const float *bestpeak=*bestpeakindex;
	if (peak>0) {
		int peakindex=bestpeak-autocorr;
		pdetector->confidence = (*bestpeak) * pdetector->acwinv[peakindex];
		if (bestpeak<end) {
			//Parabolically interpolate to find the peak
			int denominator=2*bestpeak[0]-bestpeak[1]-bestpeak[-1];
			if(denominator!=0) {
				int numerator=bestpeak[1]-bestpeak[-1];
				float fmax=peakindex+((float)numerator)/((float)denominator);
				pperiod=fmax/fs;
			} else {
				pperiod=peakindex/fs;
			}
		} else {
			pperiod = (float)(peakindex)/fs;
		}
	} else {
		pdetector->confidence=0;
	}

	// Convert to semitones
	if (pdetector->confidence>=*pdetector->p_vthresh) {
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
