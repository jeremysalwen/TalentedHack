#include "formant_corrector.h"

void FormantCorrectorInit(FormantCorrector * fcorrector, unsigned long SampleRate, int cbsize) {
	// Initialize formant corrector
	fcorrector->ford = 7; // should be sufficient to capture formants
	fcorrector->falph = pow(0.001, (float) 80 / (SampleRate));
	fcorrector->flamb = -(0.8517*sqrt(atan(0.06583*SampleRate))-0.1916); // or about -0.88 @ 44.1kHz
	fcorrector->fk = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fb = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fc = calloc(fcorrector->ford, sizeof(float));
	fcorrector->frb = calloc(fcorrector->ford, sizeof(float));
	fcorrector->frc = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fsig = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fsmooth = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fhp = 0;
	fcorrector->flp = 0;
	fcorrector->flpa = pow(0.001, (float) 10 / (SampleRate));
	fcorrector->fbuff = (float**) malloc((fcorrector->ford)*sizeof(float*));
	int i;
	for (i=0; i<fcorrector->ford; i++) {
		fcorrector->fbuff[i] = calloc(cbsize, sizeof(float));
	}
	fcorrector->ftvec = calloc(fcorrector->ford, sizeof(float));
	fcorrector->fmute = 1;
	fcorrector->fmutealph = pow(0.001, (float)1 / (SampleRate));
}

// tf is signal input
void RemoveFormants(FormantCorrector * fcorrector, CircularBuffer* buffer, float tf) {
	float foma=1-fcorrector->falph;
	// Somewhat experimental formant corrector
	//  formants are removed using an adaptive pre-filter and
	//  re-introduced after pitch manipulation using post-filter
	float fa = tf - fcorrector->fhp; // highpass pre-emphasis filter
	fcorrector->fhp = tf;
	float fb = fa;
	
	int i;
	for (i=0; i<(fcorrector->ford); i++) {
		fcorrector->fsig[i] = fa*fa*foma + fcorrector->fsig[i]*fcorrector->falph;
		float fc = (fb-(fcorrector->fc[i]))*(fcorrector->flamb) + fcorrector->fb[i];
		fcorrector->fc[i] = fc;
		fcorrector->fb[i] = fb;
		float fk_tmp = fa*fc*foma + fcorrector->fk[i]*fcorrector->falph;
		fcorrector->fk[i] = fk_tmp;
		tf = fk_tmp/(fcorrector->fsig[i] + 0.000001);
		tf = tf*foma + fcorrector->fsmooth[i]*fcorrector->falph;
		fcorrector->fsmooth[i] = tf;
		fcorrector->fbuff[i][buffer->cbiwr] = tf;
		fb = fc - tf*fa;
		fa = fa - tf*fc;
	}
	buffer->cbf[buffer->cbiwr] = fa;
	// Now hopefully the formants are reduced
	// More formant correction code at the end of the DSP loop
}
float FormantCorrectorIteration(FormantCorrector* fcorrector, float fa, long int writepoint) {
	float fb=fa;
	int i;
	for (i=0; i<fcorrector->ford; i++) {
		float fc = (fb-fcorrector->frc[i])*fcorrector->frlamb + fcorrector->frb[i];
		float tf = fcorrector->fbuff[i][writepoint];
		fb = fc - tf*fa;
		fcorrector->ftvec[i] = tf*fc;
		fa = fa - fcorrector->ftvec[i];
	}
	float result = -fa;
	for (i=(fcorrector->ford)-1; i>=0; i--) {
		result += fcorrector->ftvec[i];
	}
	return result;
}
float AddFormants(FormantCorrector * fcorrector, float in,long int writepoint) {
	// The second part of the formant corrector
	// This is a post-filter that re-applies the formants, designed
	//   to result in the exact original signal when no pitch
	//   manipulation is performed.
	// tf is signal input
	// gotta run it 3 times because of a pesky delay free loop
	//  first time: compute 0-response
    float f0resp=FormantCorrectorIteration(fcorrector,0,writepoint);
	//  second time: compute 1-response
	float f1resp=FormantCorrectorIteration(fcorrector,1,writepoint);
	//  now solve equations for output, based on 0-response and 1-response
	float tf2 = (float)2*in;
	float denominator = (float)1 - f1resp + f0resp;
	if (denominator!=0) {
		tf2 = (tf2 + f0resp) / denominator;
	}
	else {
		tf2 = 0;
	}
	//  third time: update delay registers
	float fa = tf2;
	float fb = fa;
	int i=0;
	for (i=0; i<fcorrector->ford; i++) {
		float fc = (fb-fcorrector->frc[i])*fcorrector->frlamb + fcorrector->frb[i];
		fcorrector->frc[i] = fc;
		fcorrector->frb[i] = fb;
		float tf = fcorrector->fbuff[i][writepoint];
		fb = fc - tf*fa;
		fa = fa - tf*fc;
	}
	
	tf2 = tf2 + fcorrector->flpa*fcorrector->flp;  // lowpass post-emphasis filter
	fcorrector->flp = tf2;
	
	// Bring up the gain slowly when formant correction goes from disabled
	// to enabled, while things stabilize.
	if (fcorrector->fmute>0.5) {
		tf2 = tf2*(fcorrector->fmute - 0.5)*2;
	}
	else {
		tf2 = 0;
	}
	fcorrector->fmute = (1-fcorrector->fmutealph) + fcorrector->fmutealph*fcorrector->fmute;
	// now tf is signal output
	// ...and we're done messing with formants
	return tf2;
}

void UpdateFormantWarp(FormantCorrector* fcorrector) {
	float f = pow((float)2,(*fcorrector->p_Fwarp)/2)*(1+fcorrector->flamb)/(1-fcorrector->flamb);
	fcorrector->frlamb = (f - 1)/(f + 1);
}