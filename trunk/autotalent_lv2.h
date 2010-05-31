#ifndef AUTOTALENT_LADSPA_H
#define AUTOTALENT_LADSPA_H

//#define DEBUGPLOT

#include "autotalent_formant_corrector.h"
#include "autotalent_pitch_detector.h"
#include "autotalent_pitch_shifter.h"
#include "autotalent_quantizer.h"
#include "autotalent_lfo.h"
#include <lv2.h>
#include "event.h"

#ifdef DEBUGPLOT
#include <plot.h>
#endif

#define AUTOTALENT_URI "http://jeremy.salwen/plugins/autotalent"
// The port numbers

#define AT_MIDI_OUT 0
#define AT_MIDI_IN 1
#define AT_AUDIO_IN 2
#define AT_AUDIO_OUT 3
#define AT_MIX 4
#define AT_FCORR 5
#define AT_FWARP 6
#define AT_CORR_MIDIOUT 7
#define AT_LFO_QUANT 8
#define AT_LFO_AMP 9
#define AT_LFO_RATE 10
#define AT_LFO_SHAPE 11
#define AT_LFO_SYMM 12
#define AT_AREF 13
#define AT_PULLPITCH_AMOUNT 14
#define AT_DA 15
#define AT_DAA 16
#define AT_DB 17
#define AT_DC 18
#define AT_DCC 19
#define AT_DD 20
#define AT_DDD 21
#define AT_DE 22
#define AT_DF 23
#define AT_DFF 24
#define AT_DG 25
#define AT_DGG 26
#define AT_OA 27
#define AT_OAA 28
#define AT_OB 29
#define AT_OC 30
#define AT_OCC 31
#define AT_OD 32
#define AT_ODD 33
#define AT_OE 34
#define AT_OF 35
#define AT_OFF 36
#define AT_OG 37
#define AT_OGG 38
#define AT_ACCEPT_MIDI 39
#define AT_LATENCY 40




/*************************
 *  THE MEMBER VARIABLES *
 *************************/

typedef struct {

	float* p_mix;
	float* p_InputBuffer;
	float* p_OutputBuffer;
	float* p_latency;
	unsigned char* p_correct_midiout;
	fft_vars* fmembvars; // member variables for fft routine

	unsigned long fs; // Sample rate
    int noverlap;
	
	FormantCorrector fcorrector;
	PitchDetector pdetector;
	PitchShifter pshifter;
	Quantizer quantizer;
	LFO lfo;
	CircularBuffer buffer;

#ifdef DEBUGPLOT
	int handle;
#endif
} Autotalent;

#endif
