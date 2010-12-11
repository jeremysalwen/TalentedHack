#ifndef TALENTENEDHACK_CIRCULAR_BUFFER_H
#define TALENTENEDHACK_CIRCULAR_BUFFER_H
#include <stdlib.h>

typedef struct {
	unsigned long cbsize; // size of circular buffer
	unsigned long corrsize; // cbsize/2 + 1
	unsigned long cbiwr;  //write pointer;
	float* cbi;
	float* cbf; // circular formant correction buffer	
} CircularBuffer;

void InstantiateCircularBuffer(CircularBuffer* buffer, unsigned long samplerate);
#endif