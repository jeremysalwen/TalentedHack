#ifndef TALENTENEDHACK_PITCH_DETECTOR_H
#define TALENTENEDHACK_PITCH_DETECTOR_H

#include <math.h>
#include "circular_buffer.h"
#include "fft.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef DEBUGPLOT
#include <SDL/SDL.h>
#endif

#define PI (float)3.14159265358979323846
typedef struct {
  float pperiod;
  float confidence;

  unsigned long corrsize; // Size of the autocorrelation buffers
	
  float* cbwindow;  //cosine window;
  float* acwinv; // inverse of autocorrelation of window

  float* p_pmax; // Maximum allowable pitch (Hz)
  float* p_pmin; // Minimum allowable pitch (Hz)
  float* p_vthresh; // The voiced confidence (unbiased peak) threshold level
  float* p_ppickthresh; // MPM's "k" constant
} PitchDetector;

const float * obtain_autocovariance(PitchDetector * pdetector,fft_vars* fftvars, CircularBuffer* buffer, long int N);

float get_pitch_period(PitchDetector * pdetector, const float* autocorr, unsigned long Nf, float fs);

void InstantiatePitchDetector(PitchDetector * pdetector, fft_vars* fftvars, unsigned long corrsize, double samplerate );
#endif
