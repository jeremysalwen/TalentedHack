#include "autotalent_lfo.h"

float LFOval(LFO *lfo, int noverlap, long int fs, long int N) {
	// LFO logic
	float tf = (*lfo->p_rate)*N/(noverlap*fs);
	if (tf>1) {
		tf=1;
	}
	lfo->phase = lfo->phase + tf;
	if (lfo->phase>1) {
		lfo->phase = lfo->phase-1;
	}
	float result=lfo->phase;
	tf = ((*lfo->p_symm) + 1)/2;
	if (tf<=0 || tf>=1) {
		if (tf<=0) {
			result = 1-result;
		} else {
			if (result<=tf) {
				result /= tf;
			}
			else {
				result = 1 - (result-tf)/(1-tf);
			}
		}
	}
	if ((*lfo->p_shape)>=0) {
		// linear combination of cos and line
		result = (0.5 - 0.5*cos(result*PI))*(*lfo->p_shape) + result*(1-(*lfo->p_shape));
		return (*lfo->p_amp)*(result*2 - 1);
	} else {
		// smoosh the sine horizontally until it's squarish
		tf = 1 + (*lfo->p_shape);
		if (tf<0.001) {
			result = (result - 0.5)*2/0.001;
		}
		else {
			result = (result - 0.5)*2/tf;
		}
		if (result>1) {
			result = 1;
		}
		if (result<-1) {
			result = -1;
		}
		return (*lfo->p_amp)*sin(result*PI*0.5);
	}
}

void InstantiateLFO(LFO* lfo) {
	lfo->phase = 0;
};

inline float addquantizedLFO(LFO* lfo, float output, float fs, int noverlap, unsigned int N) {
	if ((*lfo->p_quant)>=1) {
		output += (int)(12 * LFOval(lfo,fs,noverlap,N));
	}
	return output;
}

inline float addunquantizedLFO(LFO* lfo, float output, float fs, int noverlap, unsigned int N) {
	if (*lfo->p_quant<=0) {
		output+=LFOval(lfo,fs,noverlap,N)*2;
	}

	if (output<-36) {
		return -36;
	}
	if (output>24) {
		return 24;
	}
	return output;
}