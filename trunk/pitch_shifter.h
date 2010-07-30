#ifndef AUTOTALENT_PITCH_SHIFTER_H
#define AUTOTALENT_PITCH_SHIFTER_H

#define PI (float)3.14159265358979323846

#include <stdlib.h>
#include <math.h>
#include "pitch_detector.h"
#include "circular_buffer.h"
 typedef struct {
// VARIABLES FOR PITCH SHIFTER
  float phprdd; // default (unvoiced) phase period
  double inphinc; // input phase increment
  double outphinc; // input phase increment
  double phincfact; // factor determining output phase increment
	 
  double phasein;
  double phaseout;
	 
  float* frag; // windowed fragment of speech
  unsigned long fragsize; // size of fragment in samples

  float* hannwindow; // length-N hann
	 
  float* cbo; // circular output buffer
  unsigned long cbord;  //read index for circular buffer;
	 
  int active;
} PitchShifter;


void PitchShifterInit(PitchShifter * pshifter, unsigned long SampleRate, unsigned long cbsize);

void ComputePitchShifterVariables(PitchShifter * pshifter, float inpperiod, float outpperiod, float fs);

float ShiftPitch(PitchShifter * pshifter, CircularBuffer * buffer, long int N);

void Interpolate(PitchShifter * pshifter, long int bounds, long int N);
#endif
