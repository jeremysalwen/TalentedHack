#ifndef AUTOTALENT_FORMANT_CORRECTOR_H
#define AUTOTALENT_FORMANT_CORRECTOR_H

#include <math.h>
#include <stdlib.h>
#include "circular_buffer.h"
 // VARIABLES FOR FORMANT CORRECTOR
 typedef struct {
  int* p_Fcorr;
  float* p_Fwarp;
	 
  int ford;
  float falph;
  float flamb;
  float frlamb;
  float* fk;
  float* fb;
  float* fc;
  float* frb;
  float* frc;
  float* fsig;
  float* fsmooth;
  float fhp;
  float flp;
  float flpa;
  float** fbuff;
  float* ftvec;
  float fmute;
  float fmutealph;
} FormantCorrector;

void FormantCorrectorInit(FormantCorrector * fcorrector, unsigned long SampleRate, int cbsize);
void RemoveFormants(FormantCorrector * fcorrector, CircularBuffer* buffer, float tf);
void UpdateFormantWarp(FormantCorrector* fcorrector);
float AddFormants(FormantCorrector * fcorrector, float in, long int writepoint);
#endif