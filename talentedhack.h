#ifndef TALENTENEDHACK_LADSPA_H
#define TALENTENEDHACK_LADSPA_H

//#define DEBUGPLOT

#include "formant_corrector.h"
#include "pitch_detector.h"
#include "pitch_shifter.h"
#include "quantizer.h"
#include "lfo.h"
#include "pitch_smoother.h"
#include <lv2.h>
#include "event.h"

#ifdef DEBUGPLOT
#include "SDL/SDL.h"
#endif

#define TALENTEDHACK_URI "urn:jeremy.salwen:plugins:talentedhack"
// The port numbers

#define AT_MIDI_OUT 0
#define AT_MIDI_IN 1
#define AT_AUDIO_IN 2
#define AT_AUDIO_OUT 3
#define AT_MIX 4
#define AT_PULLPITCH_AMOUNT 5
#define AT_PITCH_SMOOTH 6
#define AT_MIN_PITCH 7
#define AT_MAX_PITCH 8
#define AT_VOICED_THRESH 9
#define AT_MPM_K 10
#define AT_FCORR 11
#define AT_FWARP 12
#define AT_ACCEPT_MIDI 13
#define AT_CORR_MIDIOUT 14
#define AT_LFO_QUANT 15
#define AT_LFO_AMP 16
#define AT_LFO_RATE 17
#define AT_LFO_SHAPE 18
#define AT_LFO_SYMM 19
#define AT_AREF 20
#define AT_DA 21
#define AT_DAA 22
#define AT_DB 23
#define AT_DC 24
#define AT_DCC 25
#define AT_DD 26
#define AT_DDD 27
#define AT_DE 28
#define AT_DF 29
#define AT_DFF 30
#define AT_DG 31
#define AT_DGG 32
#define AT_OA 33
#define AT_OAA 34
#define AT_OB 35
#define AT_OC 36
#define AT_OCC 37
#define AT_OD 38
#define AT_ODD 39
#define AT_OE 40
#define AT_OF 41
#define AT_OFF 42
#define AT_OG 43
#define AT_OGG 44
#define AT_LATENCY 45



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
	PitchSmoother psmoother;
	Quantizer quantizer;
	LFO lfo;
	CircularBuffer buffer;


} TalentedHack
;
#ifdef DEBUGPLOT
	 SDL_Surface* screen;
     int printed;
#endif
#endif
