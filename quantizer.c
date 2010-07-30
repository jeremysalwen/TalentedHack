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
	q->midi_event_id = 0;
	q->event_ref = 0;
	LV2_URI_Map_Feature *map_feature;
	const LV2_Feature * const *  i;
	for (i = features; *i; i++) {
    if (!strcmp((*i)->URI, "http://lv2plug.in/ns/ext/uri-map"))
    {
      map_feature = (*i)->data;
      q->midi_event_id = map_feature->uri_to_id(map_feature->callback_data,
                                                      "http://lv2plug.in/ns/ext/event",
                                                      "http://lv2plug.in/ns/ext/midi#MidiEvent");
    } else if (!strcmp((*i)->URI, "http://lv2plug.in/ns/ext/event")) {
      q->event_ref = (*i)->data;
    }
  }
  if (q->midi_event_id == 0 || q->event_ref == NULL)
  {
    fprintf(stderr, "autotalent LV2: MIDI support not supported in host... disabling.\n");
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

