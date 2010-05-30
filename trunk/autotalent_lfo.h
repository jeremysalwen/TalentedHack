#ifndef AUTOTALENT_LFO_H
#define AUTOTALENT_LFO_H
#include <math.h>
#define PI (float)3.14159265358979323846

typedef struct {
  //Inputs
  float* p_amp;
  float* p_rate;
  float* p_shape;
  float* p_symm;
  float* p_quant;

  float phase;
} LFO;

float LFOval(LFO *lfo, int noverlap, long int fs, long int N);
void InstantiateLFO(LFO* lfo);

inline float addquantizedLFO(LFO* lfo, float output, float fs, int noverlap, unsigned int N);
inline float addunquantizedLFO(LFO* lfo, float output, float fs, int noverlap, unsigned int N);
#endif