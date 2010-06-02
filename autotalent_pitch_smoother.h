#ifndef AUTOTALENT_PITCH_SMOOTHER_H
#define AUTOTALENT_PITCH_SMOOTHER_H
#include <math.h>
typedef struct {
	float * p_pitchsmooth;
	float lastpitch;
	float momentum;
	float periods_per_second;
} PitchSmoother;

void InitializePitchSmoother(PitchSmoother* s, unsigned long N, int noverlap, float fs);
float SmoothPitch(PitchSmoother* s, float semitones);
void ResetPitchSmoother(PitchSmoother* s);
#endif