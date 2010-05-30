OBJS = autotalent_circular_buffer.o  autotalent_midi.o autotalent_fft.o autotalent_pitch_detector.o autotalent_formant_corrector.o autotalent_pitch_shifter.o autotalent_lfo.o autotalent_quantizer.o autotalent_lv2.o
DEBUG =
CFLAGS = -Wall -fPIC -O3 $(DEBUG)
LDFLAGS = $(DEBUG) `pkg-config --libs fftw3f` -shared

all: autotalent_lv2.so

autotalent_lv2.so: $(OBJS)
	ld $(LDFLAGS) $(OBJS) -o autotalent_lv2.so
	
clean:
	rm *.so
	rm *.o