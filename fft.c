#include <stdlib.h>
#include "fft.h"

// Constructor for FFT routine
fft_vars* fft_con(int nfft) {
	fft_vars* fftvars = (fft_vars*) malloc(sizeof(fft_vars));

	fftvars->nfft = nfft;
	fftvars->numfreqs = nfft/2 + 1;

	fftvars->ffttime = fftwf_malloc(nfft* sizeof(float));
	fftvars->complex = fftwf_malloc(fftvars->numfreqs* sizeof(fftwf_complex));
	fftvars->forwardplan=fftwf_plan_dft_r2c_1d(nfft,fftvars->ffttime, fftvars->complex, FFTW_ESTIMATE);
	fftvars->reverseplan=fftwf_plan_dft_c2r_1d(nfft,fftvars->complex,fftvars->ffttime, FFTW_ESTIMATE);
	return fftvars;
}

// Destructor for FFT routine
void fft_des(fft_vars* fftvars)
{
	fftwf_destroy_plan(fftvars->forwardplan);
	fftwf_destroy_plan(fftvars->reverseplan);
	fftwf_free(fftvars->ffttime);
	fftwf_free(fftvars->complex);
	free(fftvars);
}

// Perform forward FFT of real data
// Accepts:
//   fftvars - pointer to struct of FFT variables
//   input - pointer to an array of (real) input values, size nfft
//   output_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   output_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
void fft_forward(fft_vars* fftvars)
{

	fftwf_execute(fftvars->forwardplan);

}

// Perform inverse FFT, returning real data
// Accepts:
//   fftvars - pointer to struct of FFT variables
//   input_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   input_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
//   output - pointer to an array of (real) input values, size nfft
void fft_inverse(fft_vars* fftvars)
{


	fftwf_execute(fftvars->reverseplan);
}

// DONE WITH FFT CODE

