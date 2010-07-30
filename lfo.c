#include "lfo.h"

float LFOval(LFO *lfo) {
	lfo->phase = lfo->phase + lfo->increment;
	if (lfo->phase>1) {
		lfo->phase = lfo->phase-1;
	}
	//between 0 and 1, sawtooth
	float result=lfo->phase;
	//between 0 and 1
	float tf = ((*lfo->p_symm) + 1)/2;
	if (tf<=0 || tf>=1) {
		if (tf<=0) {
			result = 1-result;
		} 
	}else if (result<=tf) {
		result /= tf;
	}	else {
		result = 1 - (result-tf)/(1-tf);
	}

	if ((*lfo->p_shape)>=0) {
		// linear combination of cos and line
		result = (0.5 - 0.5*cos(result*PI))*(*lfo->p_shape) + result*(1-(*lfo->p_shape));
		result=(*lfo->p_amp)*(result*2 - 1);
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
		result= (*lfo->p_amp)*sin(result*PI*0.5);
	}
	return result;
}

void UpdateLFO(LFO* lfo,unsigned long N, int noverlap, float fs) {
	lfo->increment = (*lfo->p_rate)*N/(noverlap*fs);
	if (lfo->increment>1) {
		lfo->increment=1;
	}
}

void InstantiateLFO(LFO* lfo) {
	lfo->phase = 0;
};

float addquantizedLFO(LFO* lfo, int notes[12], int pitch) {
	if ((*lfo->p_quant)>=1) {
		float val=12 * LFOval(lfo);
		int pitchindex= (int)val;
		int snapup=(val-pitchindex)>0;
		pitchindex+=pitch;
		pitch= SnapToKey(notes,pitchindex, snapup);//I guess we will snap up
	}
	
	return pitch;
}

float addunquantizedLFO(LFO* lfo, float output) {
	if (*lfo->p_quant<=0) {
		output+=LFOval(lfo)*2;
	}

	if (output<-36) {
		return -36;
	}
	if (output>24) {
		return 24;
	}
	return output;
}
