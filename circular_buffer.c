#include "circular_buffer.h"

void InstantiateCircularBuffer(CircularBuffer * buffer, unsigned long SampleRate) {
	if (SampleRate>=88200) {
		buffer->cbsize = 4096;
	}
	else {
		buffer->cbsize = 2048;
	}
	buffer->corrsize = buffer->cbsize / 2 + 1;

	buffer->cbi = calloc(buffer->cbsize, sizeof(float));
	buffer->cbf = calloc(buffer->cbsize, sizeof(float));
	buffer->cbiwr = 0;
}
