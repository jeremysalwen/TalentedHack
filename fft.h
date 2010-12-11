#ifndef TALENTENEDHACK_FFT_H
#define TALENTENEDHACK_FFT_H

#include <fftw3.h>

// Variables for FFT routine
typedef struct
{
  int nfft;        // size of FFT
  int numfreqs;    // number of frequencies represented (nfft/2 + 1)
  float* ffttime;
  fftwf_complex* complex;

  fftwf_plan forwardplan;
  fftwf_plan reverseplan;
} fft_vars;

fft_vars* fft_con(int nfft);

void fft_des(fft_vars* fftvars);

void fft_forward(fft_vars* fftvars);

void fft_inverse(fft_vars* fftvars);

#endif
