#ifndef AUTOTALENT_LADSPA_H
#define AUTOTALENT_LADSPA_H

#include "autotalent_formant_corrector.h"
#include "autotalent_pitch_detector.h"
// The port numbers

#define AT_TUNE 0
#define AT_FIXED 1
#define AT_PULL 2
#define AT_A 3
#define AT_Bb 4
#define AT_B 5
#define AT_C 6
#define AT_Db 7
#define AT_D 8
#define AT_Eb 9
#define AT_E 10
#define AT_F 11
#define AT_Gb 12
#define AT_G 13
#define AT_Ab 14
#define AT_AMOUNT 15
#define AT_SMOOTH 16
#define AT_SHIFT 17
#define AT_SCWARP 18
#define AT_LFOAMP 19
#define AT_LFORATE 20
#define AT_LFOSHAPE 21
#define AT_LFOSYMM 22
#define AT_LFOQUANT 23
#define AT_FCORR 24
#define AT_FWARP 25
#define AT_MIX 26
#define AT_PITCH 27
#define AT_CONF 28
#define AT_INPUT1  29
#define AT_OUTPUT1 30
#define AT_LATENCY 31





/*************************
 *  THE MEMBER VARIABLES *
 *************************/

typedef struct {

	LADSPA_Data* m_pfTune;
	LADSPA_Data* m_pfFixed;
	LADSPA_Data* m_pfPull;
	LADSPA_Data* m_pfAmount;
	LADSPA_Data* m_pfSmooth;
	LADSPA_Data* m_pfShift;
	LADSPA_Data* m_pfScwarp;
	LADSPA_Data* m_pfMix;
	LADSPA_Data* m_pfPitch;
	LADSPA_Data* m_pfConf;
	LADSPA_Data* m_pfInputBuffer1;
	LADSPA_Data* m_pfOutputBuffer1;
	LADSPA_Data* m_pfLatency;
	fft_vars* fmembvars; // member variables for fft routine

	unsigned long fs; // Sample rate

	FormantCorrector fcorrect;
	PitchDetector pdetector;
	PitchShifter pshifter;
	Notes notes;
	// VARIABLES FOR LOW-RATE SECTION
	LowRate lrate;
} Autotalent;

#endif
