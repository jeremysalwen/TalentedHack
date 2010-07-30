#ifndef AUTOTALENT_LFO_H
#define AUTOTALENT_LFO_H
#include <math.h>
#include "quantizer.h"
#define PI (float)3.14159265358979323846

typedef struct {
  //Inputs
  float* p_amp;
  float* p_rate;
  float* p_shape;
  float* p_symm;
  float* p_quant;

  float phase;

  float increment;
} LFO;

float LFOval(LFO *lfo);
void InstantiateLFO(LFO* lfo);

float addquantizedLFO(LFO* lfo, int notes[12], int pitch);

float addunquantizedLFO(LFO* lfo, float output);

void UpdateLFO(LFO* lfo,unsigned long N, int noverlap, float fs);

#endif