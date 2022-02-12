#include "quantizer.h"

#include <lv2/lv2plug.in/ns/ext/midi/midi.h>

void SendMidiCommand(Quantizer* q, int samplenum, uint8_t byte1 ,uint8_t byte2, uint8_t byte3) {
	struct LV2_Atom_MidiEvent {
		 LV2_Atom_Event event;
		 uint8_t data[4];
	};
	struct LV2_Atom_MidiEvent buffer;
	buffer.event.time.frames=samplenum;
	buffer.event.body.type = q->uris.midi_MidiEvent;
	buffer.event.body.size = 3;
	buffer.data[0]=byte1;
	buffer.data[1]=byte2;
	buffer.data[2]=byte3;
	if(! q->p_midi_out) return;
	if(!lv2_atom_sequence_append_event(q->p_midi_out, q->midi_out_capacity, (LV2_Atom_Event*)&buffer)) {
		printf("Error!  Could not write midi event to buffer!\n");
	}
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
	if(!q->p_midi_out) {
		return;
	}
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
	if (((midi_data[0]&0xF0) == 0x90) && (midi_data[1] < 128))
	{
		pitch->note=midi_data[1];
	}
	/* note off */
	else if ((midi_data[0]&0xF0)==0x80) {
		if(midi_data[1]==pitch->note) {
			pitch->note=0;
			pitch->pitchbend=0;
		}
	} else if ((midi_data[0]&0xF0)==0xE0) {
		unsigned int pitchbendint=(int)midi_data[1]+(((int)midi_data[2])<<7);
		if(pitchbendint & 8192) {  //only a single bit set, and the 0 of pitch bending
			pitch->pitchbend=(pitchbendint^8192)/(float)8192;
		} else {
			pitch->pitchbend=(pitchbendint)/(float)8191;
		}
	}
}

MidiPitch FetchLatestMidiNote(Quantizer* q, int samplenum) {
	if (! q->p_midi_in) return q->InPitch;
	LV2_ATOM_SEQUENCE_FOREACH(q->p_midi_in, iter) {
		if (iter->body.type != q->uris.midi_MidiEvent) {
			printf("got non-midi event\n");
			continue;
		}
		if(iter->time.frames > samplenum) {
			break;
		}
		uint8_t * midi_data=(uint8_t*)(iter + 1);
		
		if(iter->body.size!=3) {
			printf("wrong size midi\n");
			continue;
		}
		MidiEvtToPitch(midi_data,&q->InPitch);
	}
	return q->InPitch;
}

float semitones_to_pperiod(Quantizer* q, float semitones) {
	return pow(2,-semitones/12)/(*q->p_aref);
}

float midi_to_semitones(MidiPitch note) {
	return note.note-69+(note.pitchbend*6);
}
