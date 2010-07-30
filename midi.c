#include "quantizer.h"

void SendMidiCommand(Quantizer* q, int samplenum, uint8_t byte1 ,uint8_t byte2, uint8_t byte3) {
		uint8_t buffer[3];
		buffer[0]=byte1;
		buffer[1]=byte2;
		buffer[2]=byte3;
	if(! q->MidiOut) return;
	if(!lv2_event_write(&q->out_iterator,samplenum,0,q->midi_event_id,3, buffer)) {
		printf("Error!  Could not write midi event to buffer!\n");
	};
}
inline void SendNoteOn(Quantizer* q, int note, int samplenum) {
	SendMidiCommand(q, samplenum, 0x90,note,64);
}

inline void SendNoteOff(Quantizer* q, int note, int samplenum) {
	SendMidiCommand(q, samplenum, 0x80,note,64);
}

inline void SendPitchBend(Quantizer* q, float pitchbend, int samplenum) {
	unsigned int pitchbendint;
	if(pitchbend>=0) {
		pitchbendint=8192+round(pitchbend*8191);
	} else {
		pitchbendint=8192+round(pitchbend*8192);
	}
	SendMidiCommand(q, samplenum, 0xE0,pitchbendint & 127,(pitchbendint & 16256)>>7);
}

void SendMidiOutput(Quantizer* q, MidiPitch note, int samplenum) {
	if(q->OutPitch.note!=note.note) {
		if(q->OutPitch.note) {
			SendNoteOff(q,q->OutPitch.note,samplenum);
		}
		q->OutPitch=note;
		SendNoteOn(q,note.note,samplenum);
	}
	SendPitchBend(q,note.pitchbend,samplenum);
}

void UnVoiceMidi(Quantizer* q, int samplenum) {
	if(q->OutPitch.note!=0) {
		SendNoteOff(q, q->OutPitch.note, samplenum);
		q->OutPitch.note=0;
		q->OutPitch.pitchbend=0;
	}
}

inline void MidiEvtToPitch(uint8_t * midi_data, MidiPitch * pitch) {
	/* note on */
	if (((midi_data[0]&0x90) == 0x90) && (midi_data[1] < 128))
	{
		pitch->note=midi_data[1];
	}
	/* note off */
	else if ((midi_data[0]&0x80)==0x80) {
		if(midi_data[1]==pitch->note) {
			pitch->note=0;
			pitch->pitchbend=0;
		}
	} else if ((midi_data[0]&0xE0)==0xE0) {
		unsigned int pitchbendint=(int)midi_data[1]+(((int)midi_data[2])<<7);
		if(pitchbendint & 8192) {  //only a single bit set, and the 0 of pitch bending
			pitch->pitchbend=(pitchbendint^8192)/(float)8192;
		} else {
			pitch->pitchbend=(pitchbendint)/(float)8191;
		}
	} else {
		printf("unhandled midi event\n");
	}
}

MidiPitch FetchLatestMidiNote(Quantizer* q, int samplenum) {
	if (! q->MidiIn) return q->InPitch;
	while(lv2_event_is_valid(&q->in_iterator)) {
		LV2_Event *in=lv2_event_get(&q->in_iterator,NULL);
		if (in->type == 0) {
			q->event_ref->lv2_event_unref(q->event_ref->callback_data, in);
		} else if (in->type == q->midi_event_id) {
			if(in->frames<=samplenum) {
				uint8_t * midi_data=(uint8_t *) in + sizeof(LV2_Event);
				if(in->size==3) {
					MidiEvtToPitch(midi_data,&q->InPitch);
				} else {
					printf("wrong size midi\n");
				}
			} else {
				break;
			}
		} else {
			printf("got non-midi event\n");
		}
		lv2_event_increment(&q->in_iterator);
	}
	return q->InPitch;
}

float semitones_to_pperiod(Quantizer* q, float semitones) {
	return pow(2,-semitones/12)/(*q->p_aref);
}

float midi_to_semitones(MidiPitch note) {
	return note.note-69+(note.pitchbend*6);
}
