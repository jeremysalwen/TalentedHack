#ifndef TALENTENEDHACK_FORMANT_CORRECTOR_H
#define TALENTENEDHACK_FORMANT_CORRECTOR_H

#include <math.h>
#include <stdlib.h>
#include "circular_buffer.h"
 // VARIABLES FOR FORMANT CORRECTOR

typedef struct {
	float k;
	float b;
	float c;
	float rb;
	float rc;
	float sig;
	float smooth;
	float* buff;
} FormantLevel;

 typedef struct {
  int* p_Fcorr;
  float* p_Fwarp;
	 
  int ford;	 
  float falph;
  float flamb;
  float frlamb;
  FormantLevel* flevels;
  float fhp;
  float flp;
  float flpa;
  float fmute;
  float fmutealph;
} FormantCorrector;


void FormantCorrectorInit(FormantCorrector * fcorrector, unsigned long SampleRate, int cbsize);
void RemoveFormants(FormantCorrector * fcorrector, CircularBuffer* buffer, float tf);
void UpdateFormantWarp(FormantCorrector* fcorrector);
float AddFormants(FormantCorrector * fcorrector, float in, long int writepoint);
void CleanupFormantCorrector(FormantCorrector * fcorrector);
#endif