#include "formant_corrector.h"

void FormantCorrectorInit(FormantCorrector * fcorrector, unsigned long SampleRate, int cbsize) {
	// Initialize formant corrector
	fcorrector->ford = 7; // should be sufficient to capture formants
	fcorrector->falph = pow(0.001, (float) 80 / (SampleRate));
	fcorrector->flamb = -(0.8517*sqrt(atan(0.06583*SampleRate))-0.1916); // or about -0.88 @ 44.1kHz
    fcorrector->flevels=calloc(fcorrector->ford, sizeof(FormantLevel));
	fcorrector->fhp = 0;
	fcorrector->flp = 0;
	fcorrector->flpa = pow(0.001, (float) 10 / (SampleRate));
	int i;
	for (i=0; i<fcorrector->ford; i++) {
		fcorrector->flevels[i].buff = calloc(cbsize, sizeof(float));
	}
	fcorrector->fmute = 1;
	fcorrector->fmutealph = pow(0.001, (float)1 / (SampleRate));
}


float FormantRemovalIteration(FormantLevel* level, float falph, float flamb, float *fa, float* fb) {
        float foma=(1-falph);
	    level->sig = (*fa)*(*fa)*foma + level->sig*falph;
		float fc = (*fb-(level->c))*(flamb) + level->b;

		level->c=fc;
	    level->b=*fb;
		float fk_tmp = (*fa)*fc*foma + level->k*falph;
		level->k = fk_tmp;
		float result = fk_tmp/(level->sig+ 0.000001);
		result = result*foma + level->smooth*falph;
		level->smooth = result;
		*fb = fc - result*(*fa);
		*fa = *fa - result*fc;
	    return result;
}
// tf is signal input
void RemoveFormants(FormantCorrector * fcorrector, CircularBuffer* buffer, float tf) {
    // Somewhat experimental formant corrector
	//  formants are removed using an adaptive pre-filter and
	//  re-introduced after pitch manipulation using post-filter
                    
	float fa = tf - fcorrector->fhp; // highpass pre-emphasis filter
    fcorrector->fhp = fa;
    float fb = fa;
	
	int i;
	for (i=0; i<(fcorrector->ford); i++) {
		FormantLevel* level=&fcorrector->flevels[i];
		level->buff[buffer->cbiwr]=FormantRemovalIteration(level,fcorrector->falph, fcorrector->flamb,&fa,&fb);
	}
	buffer->cbf[buffer->cbiwr] = fa;
	// Now hopefully the formants are reduced
	// More formant correction code at the end of the DSP loops
}
float FormantCorrectorIteration(FormantCorrector* fcorrector, float fa, long int writepoint) {
	float fb=fa;
	for (int i=0; i<fcorrector->ford; i++) {
		FormantLevel level=fcorrector->flevels[i];
		float fc = (fb-level.rc)*fcorrector->frlamb + level.rb;
		float tf = level.buff[writepoint];
		fb = fc - tf*fa;
		float tvec=tf*fc;
		fa -= tvec;
	}
	return -2*fa; //This is a very strange optimization.  For some reason he stores all of the ftvec values, and goes through a loop, adding it to -fa, instead of just multiplying fa by -2...
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
		FormantLevel level=fcorrector->flevels[i];
		float fc = (fb-level.rc)*fcorrector->frlamb + level.rb;
		level.rc = fc;
		level.rb = fb;
		float tf = level.buff[writepoint];
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

void CleanupFormantCorrector(FormantCorrector * fcorrector) {
	int i;
  	for (i=0; i<fcorrector->ford; i++) {
  		free(fcorrector->flevels[i].buff);
  	}
  	free(fcorrector->flevels);
}