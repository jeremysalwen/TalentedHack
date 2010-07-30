#include "pitch_smoother.h"
float SmoothPitch(PitchSmoother* s, float semitones) {
	float lastpitch=s->lastpitch;
	if(lastpitch==0 || *s->p_pitchsmooth==0) {
		s->lastpitch=semitones;
		return semitones;
	}
	float diff=semitones-lastpitch;
	float divisor=(*s->p_pitchsmooth)*s->periods_per_second;
	float maxdiff=0.04;
	if(divisor>1 && fabs(diff)>maxdiff) {
		float toadd=diff/divisor;
		if(fabs(s->momentum)<fabs(toadd)){
			s->momentum=toadd;
			s->lastpitch+=toadd;
		} else if(fabs(s->momentum)>fabs(diff)) {
			s->momentum=0;
			s->lastpitch=semitones;
		} else {
			s->lastpitch+=s->momentum;
		}
	}else {
		s->momentum=0;
		s->lastpitch=semitones;
	}
	return s->lastpitch;
}
void ResetPitchSmoother(PitchSmoother* s) {
	s->momentum=0;
	s->lastpitch=0;
}
void InitializePitchSmoother(PitchSmoother* s, unsigned long N, int noverlap, float fs) {
	s->periods_per_second=(float)(fs*noverlap)/(float)(N);
}