#include "pitch_shifter.h"
#include <stdio.h>
void PitchShifterInit(PitchShifter * pshifter, unsigned long SampleRate, unsigned long cbsize) {
	// Pitch shifter initialization
	pshifter->phprdd = 0.01; // Default period
	pshifter->inphinc = (float)1/(pshifter->phprdd * SampleRate);
	pshifter->phincfact = 1;
	pshifter->phasein = 0;
	pshifter->phaseout = 0;
	pshifter->frag = calloc(cbsize, sizeof(float));
	pshifter->fragsize = 0;

	// Standard raised cosine window, max height at N/2
	pshifter->hannwindow = calloc(cbsize, sizeof(float));
	long int i;
	for (i=0; i<cbsize; i++) {
		pshifter->hannwindow[i] = -0.5*cos(2*PI*i/cbsize) + 0.5;
	}

	pshifter->cbo = calloc(cbsize, sizeof(float));
	pshifter->cbord = 0;
}

void ComputePitchShifterVariables(PitchShifter * pshifter,float inpperiod, float outpperiod, float fs) {
	float invinphinc=inpperiod*fs;
	pshifter->inphinc = 1/invinphinc; //This is like the fraction of a period every sample is.
	pshifter->outphinc = 1/(outpperiod*fs); 
	pshifter->phincfact = pshifter->outphinc*invinphinc;

}

float ShiftPitch(PitchShifter * pshifter, CircularBuffer * buffer, long int N) {
	// Pitch shifter (kind of like a pitch-synchronous version of Fairbanks' technique)
	//   Note: pitch estimate is naturally N/2 samples old
	pshifter->phasein = pshifter->phasein + pshifter->inphinc; //This is like the total amount of the period we've been through.
	
	pshifter->phaseout = pshifter->phaseout + pshifter->outphinc; //likewise for output

	//   When input phase resets, take a snippet from N/2 samples in the past
	if (pshifter->phasein >= 1) {
		pshifter->phasein = pshifter->phasein - 1;
		long fragment_beginning= buffer->cbiwr - N/2;
		 for (long i=-N/2; i<N/2; i++) {
			pshifter->frag[(i+N)%N] = buffer->cbf[(i + fragment_beginning + N)%N];
      }
	}

	//   When output phase resets, put a snippet N/2 samples in the future
	if (pshifter->phaseout >= 1) {
		pshifter->fragsize = pshifter->fragsize*2;
		if (pshifter->fragsize > N) {
			pshifter->fragsize = N;
		}
		pshifter->phaseout = pshifter->phaseout - 1;
		long int ti3 = (long int)(((float)pshifter->fragsize) / pshifter->phincfact);

		//Interpolator
		Interpolate(pshifter, ti3, N);
		pshifter->fragsize = 0;
	}
	pshifter->fragsize++;

	//   Get output signal from buffer
	float tf = pshifter->cbo[pshifter->cbord]; // read buffer

	pshifter->cbo[pshifter->cbord] = 0; // erase for next cycle
	pshifter->cbord++; // increment read pointer
	if (pshifter->cbord >= N) {
		pshifter->cbord = 0;
	}
	return tf;
}

void Interpolate(PitchShifter * pshifter, long int bound, long int N) {
	if (bound>=N/2) {
		bound = N/2 - 1;
	}
	long int i;
	for (i=-bound/2; i<(bound/2); i++) {
		float tf = pshifter->hannwindow[(long int)N/2 + i*(long int)N/bound];
		// 3rd degree polynomial interpolator - based on eqns from Hal Chamberlin's book
		float indd = pshifter->phincfact*i;
		int ind1 = (int)indd;
		int ind2 = ind1+1;
		int ind3 = ind1+2;
		int ind0 = ind1-1;
		float val0 = pshifter->frag[(ind0+N)%N];
		float val1 = pshifter->frag[(ind1+N)%N];
		float val2 = pshifter->frag[(ind2+N)%N];
		float val3 = pshifter->frag[(ind3+N)%N];
		float vald = 0;
		vald = vald - (float)0.166666666667 * val0 * (indd - ind1) * (indd - ind2) * (indd - ind3);
		vald = vald + (float)0.5 * val1 * (indd - ind0) * (indd - ind2) * (indd - ind3);
		vald = vald - (float)0.5 * val2 * (indd - ind0) * (indd - ind1) * (indd - ind3);
		vald = vald + (float)0.166666666667 * val3 * (indd - ind0) * (indd - ind1) * (indd - ind2);
		pshifter->cbo[(i + pshifter->cbord + 3*N/2)%N] = pshifter->cbo[(i + pshifter->cbord + 3*N/2)%N] + vald*tf;
	}
}