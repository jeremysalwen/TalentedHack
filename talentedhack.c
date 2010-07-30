/* talentedhack.c
   An auto-tuning LV2 plugin.
   
   by Jeremy A. Salwen
   VERSION 1.82
   Jul 30, 2010

   Based on Autotalent, which is

   Free software by Thomas A. Baran.
   http://web.mit.edu/tbaran/www/autotalent.html
   VERSION 0.2
   March 20, 2010

  
	   
   This program is free software; you can redistribute it and/or modify        
   it under the terms of the GNU General Public License as published by        
   the Free Software Foundation; either version 2 of the License, or           
   (at your option) any later version.                                         
                                                                                
   This program is distributed in the hope that it will be useful,             
   but WITHOUT ANY WARRANTY; without even the implied warranty of              
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               
   GNU General Public License for more details.                                
                                                                                
   You should have received a copy of the GNU General Public License           
   along with this program; if not, write to the Free Software                 
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  

 */

#include "talentedhack.h"

static LV2_Descriptor *TalentedHackDescriptor = NULL;

#ifdef DEBUGPLOT
int updateScreen() {
	printed=0;
	while(1) {
		if(printed) {
		    SDL_Flip(screen);
			printed=0;
		}
		SDL_Delay(2);
	}
	return 0;
}
#endif
static void cleanupTalentedHack(LV2_Handle instance)
{
	TalentedHack * ATInstance=(TalentedHack*)instance;
	fft_des(ATInstance->fmembvars);
 	free(ATInstance->buffer.cbi);
	free(ATInstance->buffer.cbf);
	free(ATInstance->pshifter.cbo);
	free(ATInstance->pdetector.cbwindow);
	free(ATInstance->pshifter.hannwindow);
	free(ATInstance->pdetector.acwinv);
	free(ATInstance->pshifter.frag);
	free(ATInstance->fcorrector.fk);
	free(ATInstance->fcorrector.fb);
 	free(ATInstance->fcorrector.fc);
 	free(ATInstance->fcorrector.frb);
 	free(ATInstance->fcorrector.frc);
 	free(ATInstance->fcorrector.fsmooth);
 	free(ATInstance->fcorrector.fsig);
	int i;
  	for (i=0; i<ATInstance->fcorrector.ford; i++) {
  		free(ATInstance->fcorrector.fbuff[i]);
  	}
  	free(ATInstance->fcorrector.fbuff);
  	free(ATInstance->fcorrector.ftvec);
#ifdef DEBUGPLOT
	SDL_FreeSurface(screen);
    //Quit SDL
    SDL_Quit();
#endif
	free(ATInstance);
}
static void connectPortTalentedHack(LV2_Handle instance, uint32_t port, void *data)
{
	TalentedHack *plugin = (TalentedHack *)instance;
	switch (port) {
		case AT_MIDI_OUT:
			plugin->quantizer.MidiOut=data;
			break;
		case AT_MIDI_IN:
			plugin->quantizer.MidiIn=data;
			break;
		case AT_AUDIO_IN:
			plugin->p_InputBuffer=data;
			break;
		case AT_AUDIO_OUT:
			plugin->p_OutputBuffer=data;
			break;
		case AT_MIX:
			plugin->p_mix=data;
			break;
		case AT_FCORR:
			plugin->fcorrector.p_Fcorr=data;
			break;
		case AT_FWARP:
			plugin->fcorrector.p_Fwarp=data;
			break;
		case AT_CORR_MIDIOUT:
			plugin->p_correct_midiout=data;
			break;
		case AT_LFO_QUANT:
			plugin->lfo.p_quant=data;
			break;
		case AT_LFO_AMP:
			plugin->lfo.p_amp=data;
			break;
		case AT_LFO_RATE:
			plugin->lfo.p_rate=data;
			break;
		case AT_LFO_SHAPE:
			plugin->lfo.p_shape=data;
			break;
		case AT_LFO_SYMM:
			plugin->lfo.p_symm=data;
			break;
		case AT_AREF:
			plugin->quantizer.p_aref=data;
			break;
		case AT_PULLPITCH_AMOUNT:
			plugin->quantizer.p_amount=data;
			break;
		case AT_DA:
			plugin->quantizer.inotes.A=data;
			break;
		case AT_DAA:
			plugin->quantizer.inotes.Bb=data;
			break;
		case AT_DB:
			plugin->quantizer.inotes.B=data;
			break;
		case AT_DC:
			plugin->quantizer.inotes.C=data;
			break;
		case AT_DCC:
			plugin->quantizer.inotes.Db=data;
			break;
		case AT_DD:
			plugin->quantizer.inotes.D=data;
			break;
		case AT_DDD:
			plugin->quantizer.inotes.Eb=data;
			break;
		case AT_DE:
			plugin->quantizer.inotes.E=data;
			break;
		case AT_DF:
			plugin->quantizer.inotes.F=data;
			break;
		case AT_DFF:
			plugin->quantizer.inotes.Gb=data;
			break;
		case AT_DG:
			plugin->quantizer.inotes.G=data;
			break;
		case AT_DGG:
			plugin->quantizer.inotes.Ab=data;
			break;
		case AT_OA:
			plugin->quantizer.onotes.A=data;
			break;
		case AT_OAA:
			plugin->quantizer.onotes.Bb=data;
			break;
		case AT_OB:
			plugin->quantizer.onotes.B=data;
			break;
		case AT_OC:
			plugin->quantizer.onotes.C=data;
			break;
		case AT_OCC:
			plugin->quantizer.onotes.Db=data;
			break;
		case AT_OD:
			plugin->quantizer.onotes.D=data;
			break;
		case AT_ODD:
			plugin->quantizer.onotes.Eb=data;
			break;
		case AT_OE:
			plugin->quantizer.onotes.E=data;
			break;
		case AT_OF:
			plugin->quantizer.onotes.F=data;
			break;
		case AT_OFF:
			plugin->quantizer.onotes.Gb=data;
			break;
		case AT_OG:
			plugin->quantizer.onotes.G=data;
			break;
		case AT_OGG:
			plugin->quantizer.onotes.Ab=data;
			break;
		case AT_ACCEPT_MIDI:
			plugin->quantizer.p_accept_midi=data;
			break;
		case AT_LATENCY:
			plugin->p_latency=data;
			break;
		case AT_PITCH_SMOOTH:
			plugin->psmoother.p_pitchsmooth=data;
			break;
		default:
			printf("Error, didn't connect port #%i",port);
	}
}

static LV2_Handle instantiateTalentedHack(const LV2_Descriptor *descriptor,
	    double s_rate, const char *path,
	    const LV2_Feature * const* features)
{
	TalentedHack *membvars = (TalentedHack *)malloc(sizeof(TalentedHack));
	InstantiateCircularBuffer(&membvars->buffer,s_rate);
	unsigned long N=membvars->buffer.cbsize;
	membvars->fmembvars = fft_con(N);
	membvars->fs = s_rate;
	membvars->noverlap = 4;
	
    InstantiatePitchDetector(&membvars->pdetector, membvars->fmembvars, N, s_rate);
	InstantiateLFO(&membvars->lfo);
	FormantCorrectorInit(&membvars->fcorrector,s_rate,N);
#ifdef DEBUGPLOT
	//Start SDL
    	SDL_Init( SDL_INIT_EVERYTHING );
    	//Set up screen
    	screen = SDL_SetVideoMode( 1024, 100, 8, SDL_HWSURFACE);
	if ( (screen->flags & SDL_HWSURFACE) != SDL_HWSURFACE ) {
     		 printf("Can't get hardware surface\n");
	}
	printf("%i bpp\n",screen->format->BitsPerPixel);
	SDL_CreateThread(updateScreen, NULL);
#endif
	
	PitchShifterInit(&membvars->pshifter, s_rate,N);
	InitializePitchSmoother(&membvars->psmoother, N, membvars->noverlap, s_rate);
	QuantizerInit(&membvars->quantizer,features);
	return membvars;
}

inline void IncrementPointer(CircularBuffer * buffer) {
	// Input write pointer logic
	buffer->cbiwr++;
	if (buffer->cbiwr >= buffer->cbsize) {
		buffer->cbiwr = 0;
	}
}

static void runTalentedHack(LV2_Handle instance, uint32_t sample_count)
{
	TalentedHack* psTalentedHack = (TalentedHack *)instance;
	
	unsigned long N = psTalentedHack->buffer.cbsize;
	unsigned long Nf = psTalentedHack->buffer.corrsize;
	float fs = psTalentedHack->fs;
	
	UpdateFormantWarp(&psTalentedHack->fcorrector);
	UpdateQuantizer(&psTalentedHack->quantizer);
	UpdateLFO(&psTalentedHack->lfo,N,psTalentedHack->noverlap,fs);
	lv2_event_begin(&psTalentedHack->quantizer.in_iterator, psTalentedHack->quantizer.MidiIn);
	lv2_event_begin(&psTalentedHack->quantizer.out_iterator, psTalentedHack->quantizer.MidiOut);
	
	const float* pfInput=psTalentedHack->p_InputBuffer;
	float* pfOutput=psTalentedHack->p_OutputBuffer;
	
	int fcorr=*(psTalentedHack->fcorrector.p_Fcorr);
	
	/*******************
	 *  MAIN DSP LOOP  *
	 *******************/
	unsigned long lSampleIndex;
	for (lSampleIndex = 0; lSampleIndex < sample_count; lSampleIndex++)  {
		// load data into circular buffer
		float in = (float) *(pfInput++);
		
		psTalentedHack->buffer.cbi[psTalentedHack->buffer.cbiwr] = in;
		if (fcorr>=1) {
			RemoveFormants(&psTalentedHack->fcorrector,&psTalentedHack->buffer,in);
		}
		else {
			psTalentedHack->buffer.cbf[psTalentedHack->buffer.cbiwr] = in;
		}
		
		IncrementPointer(&psTalentedHack->buffer);
		
		// Every N/noverlap samples, run pitch estimation / manipulation code
		if ((psTalentedHack->buffer.cbiwr)%(N/psTalentedHack->noverlap) == 0) {
			//  ---- Calculate pitch and confidence ----
			float pperiod=get_pitch_period(&psTalentedHack->pdetector, obtain_autocovariance(&psTalentedHack->pdetector,psTalentedHack->fmembvars,&psTalentedHack->buffer,N),Nf,fs);
			
			if(pperiod>0) {
				MidiPitch note;
				note=pperiod_to_midi(&psTalentedHack->quantizer,pperiod);
				if(*psTalentedHack->p_correct_midiout) {
					PullToInTune(&psTalentedHack->quantizer, &note);
				}
				
				SendMidiOutput(&psTalentedHack->quantizer,note,lSampleIndex);
				//Now we begin to modify the note, to determine what pitch we want to shift to
				MidiPitch input=FetchLatestMidiNote(&psTalentedHack->quantizer,lSampleIndex);
				note=MixMidiIn(&psTalentedHack->quantizer,note,input);
				note.note=SnapToKey(psTalentedHack->quantizer.oNotes, note.note, note.pitchbend>0);
				if(!*psTalentedHack->p_correct_midiout) {
					PullToInTune(&psTalentedHack->quantizer, &note);
				}
				
				note.note=addquantizedLFO(&psTalentedHack->lfo,psTalentedHack->quantizer.oNotes,note.note);
				
				float outpitch=midi_to_semitones(note);
				
				outpitch=addunquantizedLFO(&psTalentedHack->lfo,outpitch);
				outpitch=SmoothPitch(&psTalentedHack->psmoother,outpitch); 
				float outpperiod=semitones_to_pperiod(&psTalentedHack->quantizer, outpitch);
				// Compute variables for pitch shifter that depend on pitch
				ComputePitchShifterVariables(&psTalentedHack->pshifter, pperiod,outpperiod,fs);
				psTalentedHack->pshifter.active=1;
			} else { 
				UnVoiceMidi(&psTalentedHack->quantizer,lSampleIndex);
				ResetPitchSmoother(&psTalentedHack->psmoother);
				psTalentedHack->pshifter.active=0;
			}
		}
		
		if(psTalentedHack->pshifter.active) {
			in=ShiftPitch(&psTalentedHack->pshifter,&psTalentedHack->buffer, N);
		}
		unsigned int twoahead = (psTalentedHack->buffer.cbiwr + 2)%N;
		if (*psTalentedHack->fcorrector.p_Fcorr>=1) {
			in=AddFormants(&psTalentedHack->fcorrector,in,twoahead);
		} else {
			psTalentedHack->fcorrector.fmute = 0;
		}

		// Write audio to output of plugin
		// Mix (blend between original (delayed) =0 and processed =1)
		*(pfOutput++)=(*psTalentedHack->p_mix)*in + (1-(*psTalentedHack->p_mix))*psTalentedHack->buffer.cbi[twoahead];
		
	}
	FetchLatestMidiNote(&psTalentedHack->quantizer,sample_count-1);
	// Tell the host the algorithm latency
	*(psTalentedHack->p_latency) = (N-1);
}

static void init()
{
	TalentedHackDescriptor =
	 (LV2_Descriptor *)malloc(sizeof(LV2_Descriptor));

	TalentedHackDescriptor->URI = TALENTEDHACK_URI;
	TalentedHackDescriptor->activate = NULL;
	TalentedHackDescriptor->cleanup = cleanupTalentedHack;
	TalentedHackDescriptor->connect_port = connectPortTalentedHack;
	TalentedHackDescriptor->deactivate = NULL;
	TalentedHackDescriptor->instantiate = instantiateTalentedHack;
	TalentedHackDescriptor->run = runTalentedHack;
	TalentedHackDescriptor->extension_data = NULL;
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index)
{
	if (!TalentedHackDescriptor) init();

	switch (index) {
	case 0:
		return TalentedHackDescriptor;
	default:
		return NULL;
	}
}
