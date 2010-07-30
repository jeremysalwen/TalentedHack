#ifndef AUTOTALENT_LADSPA_H
#define AUTOTALENT_LADSPA_H

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

#define TALENTEDHACK_URI "http://jeremy.salwen/plugins/talentedhack"
// The port numbers

#define AT_MIDI_OUT 0
#define AT_MIDI_IN 1
#define AT_AUDIO_IN 2
#define AT_AUDIO_OUT 3
#define AT_MIX 4
#define AT_PULLPITCH_AMOUNT 5
#define AT_PITCH_SMOOTH 6
#define AT_FCORR 7
#define AT_FWARP 8
#define AT_ACCEPT_MIDI 9
#define AT_CORR_MIDIOUT 10
#define AT_LFO_QUANT 11
#define AT_LFO_AMP 12
#define AT_LFO_RATE 13
#define AT_LFO_SHAPE 14
#define AT_LFO_SYMM 15
#define AT_AREF 16
#define AT_DA 17
#define AT_DAA 18
#define AT_DB 19
#define AT_DC 20
#define AT_DCC 21
#define AT_DD 22
#define AT_DDD 23
#define AT_DE 24
#define AT_DF 25
#define AT_DFF 26
#define AT_DG 27
#define AT_DGG 28
#define AT_OA 29
#define AT_OAA 30
#define AT_OB 31
#define AT_OC 32
#define AT_OCC 33
#define AT_OD 34
#define AT_ODD 35
#define AT_OE 36
#define AT_OF 37
#define AT_OFF 38
#define AT_OG 39
#define AT_OGG 40
#define AT_LATENCY 41




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
