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
  float pmax; // Maximum allowable pitch period (seconds)
  float pmin; // Minimum allowable pitch period (seconds)
  unsigned long nmax; // Maximum period index for pitch prd est
  unsigned long nmin; // Minimum period index for pitch prd est
  float pperiod;
  float confidence;
  float* cbwindow;  //cosine window;
  float* acwinv; // inverse of autocorrelation of window
  
  float vthresh; // Voiced speech threshold
  float ppickthresh;	
} PitchDetector;

const float * obtain_autocovariance(PitchDetector * pdetector,fft_vars* fftvars, CircularBuffer* buffer, long int N);

float get_pitch_period(PitchDetector * pdetector, const float* autocorr, unsigned long Nf, float fs);

void InstantiatePitchDetector(PitchDetector * pdetector, fft_vars* fftvars, unsigned long corrsize, double samplerate );
#endif
