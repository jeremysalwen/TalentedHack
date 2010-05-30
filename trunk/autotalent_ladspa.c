#include "autotalent_ladspa.h"

/********************
 *  THE CONSTRUCTOR *
 ********************/

LV2_Handle instantiateAutotalent(const LV2_Descriptor * Descriptor, unsigned long SampleRate) {

}


//  Connect port
void 
connectPortToAutotalent(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation) {

	Autotalent * psAutotalent;

	psAutotalent = (Autotalent *)Instance;
	switch (Port) {
		case AT_TUNE:
			psAutotalent->m_pfTune = DataLocation;
			break;
		case AT_FIXED:
			psAutotalent->m_pfFixed = DataLocation;
			break;
		case AT_PULL:
			psAutotalent->m_pfPull = DataLocation;
			break;
		case AT_A:
			psAutotalent->m_pfA = DataLocation;
			break;
		case AT_Bb:
			psAutotalent->m_pfBb = DataLocation;
			break;
		case AT_B:
			psAutotalent->m_pfB = DataLocation;
			break;
		case AT_C:
			psAutotalent->m_pfC = DataLocation;
			break;
		case AT_Db:
			psAutotalent->m_pfDb = DataLocation;
			break;
		case AT_D:
			psAutotalent->m_pfD = DataLocation;
			break;
		case AT_Eb:
			psAutotalent->m_pfEb = DataLocation;
			break;
		case AT_E:
			psAutotalent->m_pfE = DataLocation;
			break;
		case AT_F:
			psAutotalent->m_pfF = DataLocation;
			break;
		case AT_Gb:
			psAutotalent->m_pfGb = DataLocation;
			break;
		case AT_G:
			psAutotalent->m_pfG = DataLocation;
			break;
		case AT_Ab:
			psAutotalent->m_pfAb = DataLocation;
			break;
		case AT_AMOUNT:
			psAutotalent->m_pfAmount = DataLocation;
			break;
		case AT_SMOOTH:
			psAutotalent->m_pfSmooth = DataLocation;
			break;
		case AT_SHIFT:
			psAutotalent->m_pfShift = DataLocation;
			break;
		case AT_SCWARP:
			psAutotalent->m_pfScwarp = DataLocation;
			break;
		case AT_LFOAMP:
			psAutotalent->m_pfLfoamp = DataLocation;
			break;
		case AT_LFORATE:
			psAutotalent->m_pfLforate = DataLocation;
			break;
		case AT_LFOSHAPE:
			psAutotalent->m_pfLfoshape = DataLocation;
			break;
		case AT_LFOSYMM:
			psAutotalent->m_pfLfosymm = DataLocation;
			break;
		case AT_LFOQUANT:
			psAutotalent->m_pfLfoquant = DataLocation;
			break;
		case AT_FCORR:
			psAutotalent->m_pfFcorr = DataLocation;
			break;
		case AT_FWARP:
			psAutotalent->m_pfFwarp = DataLocation;
			break;
		case AT_MIX:
			psAutotalent->m_pfMix = DataLocation;
			break;
		case AT_PITCH:
			psAutotalent->m_pfPitch = DataLocation;
			break;
		case AT_CONF:
			psAutotalent->m_pfConf = DataLocation;
			break;
		case AT_INPUT1:
			psAutotalent->m_pfInputBuffer1 = DataLocation;
			break;
		case AT_OUTPUT1:
			psAutotalent->m_pfOutputBuffer1 = DataLocation;
			break;
		case AT_LATENCY:
			psAutotalent->m_pfLatency = DataLocation;
			*(psAutotalent->m_pfLatency) = psAutotalent->cbsize - 1;
			break;
	}
}


// Called every time we get a new chunk of audio
void runAutotalent(LADSPA_Handle Instance, unsigned long SampleCount) {

}


LADSPA_Descriptor * g_psDescriptor;

// Called when first loaded
void 
_init() {

	char ** pcPortNames;
	LADSPA_PortDescriptor * piPortDescriptors;
	LADSPA_PortRangeHint * psPortRangeHints;

	g_psDescriptor
		= (LADSPA_Descriptor *)malloc(sizeof(LADSPA_Descriptor));

	if (g_psDescriptor) {

		g_psDescriptor->UniqueID
			= 4262;
		g_psDescriptor->Label
			= strdup("autotalent");
		g_psDescriptor->Properties
			= LADSPA_PROPERTY_HARD_RT_CAPABLE;
		g_psDescriptor->Name 
			= strdup("Autotalent");
		g_psDescriptor->Maker
			= strdup("Tom Baran");
		g_psDescriptor->Copyright
			= strdup("2010");
		g_psDescriptor->PortCount
			= 32;
		piPortDescriptors
			= (LADSPA_PortDescriptor *)calloc(32, sizeof(LADSPA_PortDescriptor));
		g_psDescriptor->PortDescriptors
			= (const LADSPA_PortDescriptor *)piPortDescriptors;
		piPortDescriptors[AT_TUNE]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_FIXED]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_PULL]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_A]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_Bb]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_B]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_C]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_Db]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_D]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_Eb]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_E]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_F]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_Gb]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_G]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_Ab]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_AMOUNT]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_SMOOTH]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_SHIFT]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_SCWARP]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_LFOAMP]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_LFORATE]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_LFOSHAPE]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_LFOSYMM]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_LFOQUANT]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_FCORR]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_FWARP]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_MIX]
			= LADSPA_PORT_INPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_PITCH]
			= LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_CONF]
			= LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL;
		piPortDescriptors[AT_INPUT1]
			= LADSPA_PORT_INPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[AT_OUTPUT1]
			= LADSPA_PORT_OUTPUT | LADSPA_PORT_AUDIO;
		piPortDescriptors[AT_LATENCY]
			= LADSPA_PORT_OUTPUT | LADSPA_PORT_CONTROL;
		pcPortNames
			= (char **)calloc(32, sizeof(char *));
		g_psDescriptor->PortNames 
			= (const char **)pcPortNames;
		pcPortNames[AT_TUNE]
			= strdup("Concert A (Hz)");
		pcPortNames[AT_FIXED]
			= strdup("Fixed pitch (semitones w.r.t. A)");
		pcPortNames[AT_PULL]
			= strdup("Pull to fixed pitch");
		pcPortNames[AT_A]
			= strdup("A");
		pcPortNames[AT_Bb]
			= strdup("Bb");
		pcPortNames[AT_B]
			= strdup("B");
		pcPortNames[AT_C]
			= strdup("C");
		pcPortNames[AT_Db]
			= strdup("Db");
		pcPortNames[AT_D]
			= strdup("D");
		pcPortNames[AT_Eb]
			= strdup("Eb");
		pcPortNames[AT_E]
			= strdup("E");
		pcPortNames[AT_F]
			= strdup("F");
		pcPortNames[AT_Gb]
			= strdup("Gb");
		pcPortNames[AT_G]
			= strdup("G");
		pcPortNames[AT_Ab]
			= strdup("Ab");
		pcPortNames[AT_AMOUNT]
			= strdup("Correction strength");
		pcPortNames[AT_SMOOTH]
			= strdup("Correction smoothness");
		pcPortNames[AT_SHIFT]
			= strdup("Pitch shift (scale notes)");
		pcPortNames[AT_SCWARP]
			= strdup("Output scale rotate (scale notes)");
		pcPortNames[AT_LFOAMP]
			= strdup("LFO depth");
		pcPortNames[AT_LFORATE]
			= strdup("LFO rate (Hz)");
		pcPortNames[AT_LFOSHAPE]
			= strdup("LFO shape (square->sine->tri)");
		pcPortNames[AT_LFOSYMM]
			= strdup("LFO symmetry");
		pcPortNames[AT_LFOQUANT]
			= strdup("LFO quantization");
		pcPortNames[AT_FCORR]
			= strdup("Formant correction");
		pcPortNames[AT_FWARP]
			= strdup("Formant warp");
		pcPortNames[AT_MIX]
			= strdup("Mix");
		pcPortNames[AT_PITCH]
			= strdup("Detected pitch (semitones w.r.t. A)");
		pcPortNames[AT_CONF]
			= strdup("Pitch detection confidence");
		pcPortNames[AT_INPUT1]
			= strdup("Input");
		pcPortNames[AT_OUTPUT1]
			= strdup("Output");
		pcPortNames[AT_LATENCY]
			= strdup("latency");
		psPortRangeHints = ((LADSPA_PortRangeHint *)
		                    calloc(32, sizeof(LADSPA_PortRangeHint)));
		g_psDescriptor->PortRangeHints
			= (const LADSPA_PortRangeHint *)psPortRangeHints;
		psPortRangeHints[AT_TUNE].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_440);
		psPortRangeHints[AT_TUNE].LowerBound 
			= 400;
		psPortRangeHints[AT_TUNE].UpperBound 
			= 480;
		psPortRangeHints[AT_FIXED].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_FIXED].LowerBound 
			= -36;
		psPortRangeHints[AT_FIXED].UpperBound 
			= 12;
		psPortRangeHints[AT_PULL].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_PULL].LowerBound 
			= 0;
		psPortRangeHints[AT_PULL].UpperBound 
			= 1;
		psPortRangeHints[AT_A].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_A].LowerBound 
			= -1.1;
		psPortRangeHints[AT_A].UpperBound 
			= 1.1;
		psPortRangeHints[AT_Bb].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[AT_Bb].LowerBound 
			= -1.1;
		psPortRangeHints[AT_Bb].UpperBound 
			= 1.1;
		psPortRangeHints[AT_B].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_B].LowerBound 
			= -1.1;
		psPortRangeHints[AT_B].UpperBound 
			= 1.1;
		psPortRangeHints[AT_C].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_C].LowerBound 
			= -1.1;
		psPortRangeHints[AT_C].UpperBound 
			= 1.1;
		psPortRangeHints[AT_Db].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[AT_Db].LowerBound 
			= -1.1;
		psPortRangeHints[AT_Db].UpperBound 
			= 1.1;
		psPortRangeHints[AT_D].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_D].LowerBound 
			= -1.1;
		psPortRangeHints[AT_D].UpperBound 
			= 1.1;
		psPortRangeHints[AT_Eb].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[AT_Eb].LowerBound 
			= -1.1;
		psPortRangeHints[AT_Eb].UpperBound 
			= 1.1;
		psPortRangeHints[AT_E].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_E].LowerBound 
			= -1.1;
		psPortRangeHints[AT_E].UpperBound 
			= 1.1;
		psPortRangeHints[AT_F].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_F].LowerBound 
			= -1.1;
		psPortRangeHints[AT_F].UpperBound 
			= 1.1;
		psPortRangeHints[AT_Gb].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[AT_Gb].LowerBound 
			= -1.1;
		psPortRangeHints[AT_Gb].UpperBound 
			= 1.1;
		psPortRangeHints[AT_G].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_G].LowerBound 
			= -1.1;
		psPortRangeHints[AT_G].UpperBound 
			= 1.1;
		psPortRangeHints[AT_Ab].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_MINIMUM);
		psPortRangeHints[AT_Ab].LowerBound 
			= -1.1;
		psPortRangeHints[AT_Ab].UpperBound 
			= 1.1;
		psPortRangeHints[AT_AMOUNT].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_1);
		psPortRangeHints[AT_AMOUNT].LowerBound 
			= 0;
		psPortRangeHints[AT_AMOUNT].UpperBound 
			= 1;
		psPortRangeHints[AT_SMOOTH].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_SMOOTH].LowerBound 
			= 0;
		psPortRangeHints[AT_SMOOTH].UpperBound 
			= 1;
		psPortRangeHints[AT_SHIFT].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_SHIFT].LowerBound 
			= -12;
		psPortRangeHints[AT_SHIFT].UpperBound 
			= 12;
		psPortRangeHints[AT_SCWARP].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_INTEGER |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_SCWARP].LowerBound 
			= -5.1;
		psPortRangeHints[AT_SCWARP].UpperBound 
			= 5.1;
		psPortRangeHints[AT_LFOAMP].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_LFOAMP].LowerBound 
			= 0;
		psPortRangeHints[AT_LFOAMP].UpperBound 
			= 1;
		psPortRangeHints[AT_LFORATE].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_MIDDLE);
		psPortRangeHints[AT_LFORATE].LowerBound 
			= 0;
		psPortRangeHints[AT_LFORATE].UpperBound 
			= 10;
		psPortRangeHints[AT_LFOSHAPE].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_LFOSHAPE].LowerBound 
			= -1;
		psPortRangeHints[AT_LFOSHAPE].UpperBound 
			= 1;
		psPortRangeHints[AT_LFOSYMM].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_LFOSYMM].LowerBound 
			= -1;
		psPortRangeHints[AT_LFOSYMM].UpperBound 
			= 1;
		psPortRangeHints[AT_LFOQUANT].HintDescriptor
			= (LADSPA_HINT_TOGGLED |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_FCORR].HintDescriptor
			= (LADSPA_HINT_TOGGLED |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_FWARP].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_0);
		psPortRangeHints[AT_FWARP].LowerBound 
			= -1;
		psPortRangeHints[AT_FWARP].UpperBound 
			= 1;
		psPortRangeHints[AT_MIX].HintDescriptor
			= (LADSPA_HINT_BOUNDED_BELOW |
			   LADSPA_HINT_BOUNDED_ABOVE |
			   LADSPA_HINT_DEFAULT_1);
		psPortRangeHints[AT_MIX].LowerBound 
			= 0;
		psPortRangeHints[AT_MIX].UpperBound 
			= 1;
		psPortRangeHints[AT_PITCH].HintDescriptor
			= 0;
		psPortRangeHints[AT_CONF].HintDescriptor
			= 0;
		psPortRangeHints[AT_INPUT1].HintDescriptor
			= 0;
		psPortRangeHints[AT_OUTPUT1].HintDescriptor
			= 0;
		psPortRangeHints[AT_LATENCY].HintDescriptor
			= 0;

		g_psDescriptor->instantiate 
			= instantiateAutotalent;
		g_psDescriptor->connect_port 
			= connectPortToAutotalent;
		g_psDescriptor->activate
			= NULL;
		g_psDescriptor->run
			= runAutotalent;
		g_psDescriptor->run_adding
			= NULL;
		g_psDescriptor->set_run_adding_gain
			= NULL;
		g_psDescriptor->deactivate
			= NULL;
		g_psDescriptor->cleanup
			= cleanupAutotalent;
	}

}



void
deleteDescriptor(LADSPA_Descriptor * psDescriptor) {
	unsigned long lIndex;
	if (psDescriptor) {
		free((char *)psDescriptor->Label);
		free((char *)psDescriptor->Name);
		free((char *)psDescriptor->Maker);
		free((char *)psDescriptor->Copyright);
		free((LADSPA_PortDescriptor *)psDescriptor->PortDescriptors);
		for (lIndex = 0; lIndex < psDescriptor->PortCount; lIndex++)
			free((char *)(psDescriptor->PortNames[lIndex]));
		free((char **)psDescriptor->PortNames);
		free((LADSPA_PortRangeHint *)psDescriptor->PortRangeHints);
		free(psDescriptor);
	}
}


// Called when library is unloaded
void
_fini() {
	deleteDescriptor(g_psDescriptor);
}


// Return the plugin descriptor (there's only one in thfs ffile)
const LADSPA_Descriptor * 
ladspa_descriptor(unsigned long Index) {
	switch (Index) {
		case 0:
			return g_psDescriptor;
		default:
			return NULL;
	}
}


// All done
