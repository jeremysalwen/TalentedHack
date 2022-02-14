#include "quantizer.h"


void CopyNotesToBuffer(Notes* notes, int buffer[12]) {
	buffer[0] = (int) *(notes->A);
	buffer[1] = (int) *(notes->Bb);
	buffer[2] = (int) *(notes->B);
	buffer[3] = (int) *(notes->C);
	buffer[4] = (int) *(notes->Db);
	buffer[5] = (int) *(notes->D);
	buffer[6] = (int) *(notes->Eb);
	buffer[7] = (int) *(notes->E);
	buffer[8] = (int) *(notes->F);
	buffer[9] = (int) *(notes->Gb);
	buffer[10] = (int) *(notes->G);
	buffer[11] = (int) *(notes->Ab);
}

void UpdateQuantizer(Quantizer * q) {
	if(q->p_midi_out) {
		// We save the capacity of the buffer, then clear it to prepare for output.
		q->midi_out_capacity = q->p_midi_out->atom.size;
		lv2_atom_sequence_clear(q->p_midi_out);
		// Have to set the output type too?
		q->p_midi_out->atom.type = q->uris.atom_Sequence;
	}
	CopyNotesToBuffer(&q->inotes,q->iNotes);
	CopyNotesToBuffer(&q->onotes,q->oNotes);
	int numin=0;
	int numout=0;
	int i;
	for (i=0; i<12; i++) {
		if (q->iNotes[i]>=0) {
			numin++;
		}
		if (q->oNotes[i]>=0) {
			numout++;
		}
	}
	
	// If no notes are selected as being in the scale, instead snap to all notes
	if (numin==0) {
		for (i=0; i<12; i++) {
			q->iNotes[i] = 1;
		}
	}
	if (numout==0) {
		for (i=0; i<12; i++) {
			q->oNotes[i] = 1;
		}
	}
}
void PullToInTune(Quantizer* q, MidiPitch* pitch) {
	pitch->pitchbend *=(1-(*q->p_amount));
}

//saved To later implement smooth gliding.



MidiPitch semitones_to_midi(const int notes[12], float semitones) {
	int prevsemitone=floor(semitones);
	int nextsemitone=prevsemitone+1;
	while (notes[positive_mod(prevsemitone,12)]<0) {
		prevsemitone--;
	}
	// finding next higher pitch in scale
	while (notes[positive_mod(nextsemitone,12)]<0) {
		nextsemitone ++;
	}
	float lowdiff=semitones-prevsemitone; //positive
	float highdiff=semitones-nextsemitone; //negative
	MidiPitch result;
	//This is because midi pitch bend commands are not linear (!)  When you bend up, you can go to a value of 8191, but when you bend down, you can go to a value of -8192
	if(lowdiff<(-highdiff)) {//jumping down
		result.note=prevsemitone;
		result.pitchbend=lowdiff/6;
	} else {
		result.note=nextsemitone;
		result.pitchbend=highdiff/6;
	}
	result.note+=69;
	return result;
}

MidiPitch pperiod_to_midi(Quantizer* q, float pperiod) {
	float semitones=-12*log10((float)(*q->p_aref)*pperiod)*L2SC;
	return semitones_to_midi(q->iNotes,semitones);
}

void QuantizerInit(Quantizer* q, const LV2_Feature * const * features) {
	q->InPitch.note=0;
	q->OutPitch.note=0;
	q->p_midi_in = NULL;
	q->p_midi_out = NULL;
	q->midi_in_last_event_valid = false;
	q->midi_out_capacity = 0;
	
	q->uris.midi_MidiEvent = 0;
	q->uris.atom_Sequence = 0;
	for (int i = 0; features[i]; ++i) {
		if (!strcmp(features[i]->URI, LV2_URID__map)) {
		  LV2_URID_Map* map = (LV2_URID_Map*)features[i]->data;
		  q->uris.midi_MidiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);
		  q->uris.atom_Sequence = map->map(map->handle, LV2_ATOM__Sequence);
		  break;
		}
		
	}
	if (q->uris.midi_MidiEvent == 0) {
		fprintf(stderr, "TalentedHack: MIDI support not supported in host... disabling.\n");
	}
}

MidiPitch MixMidiIn(Quantizer* q, MidiPitch detected, MidiPitch in) {
	if(*q->p_accept_midi>0 && in.note>0) {
		return in;
	} else {
		return detected;
	}
}


int SnapToKey(int notes[12], int note, int snapup) {
	int index=note -69;
	if(notes[positive_mod(index,12)]>=0) {
		return note;
	}
	int lower=index-1;
	int higher=index+1;
	while(notes[positive_mod(lower,12)]<0) {
		lower--;
	}
	while(notes[positive_mod(higher,1)]<0) {
		higher++;
	}
	if(higher-index<index-lower) {
		return higher+69;
	}
	if(higher-index>index-lower) {
		return lower+69;
	}
	if(notes[positive_mod(lower,12)]>=1) {
		return lower+69;
	}
	if(notes[positive_mod(higher,12)]>=1) {
		return higher+69;
	}
	if(snapup) {
		return higher+69;
	} else {
		return lower+69;
	}
}

