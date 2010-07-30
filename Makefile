OBJS = circular_buffer.o  midi.o fft.o pitch_detector.o formant_corrector.o pitch_shifter.o lfo.o quantizer.o talentedhack.o pitch_smoother.o
DEBUG =-g
CFLAGS = --std=c99 -Wall -fPIC `pkg-config --cflags fftw3f` -O3 $(DEBUG)
LDFLAGS = $(DEBUG) `pkg-config --libs fftw3f` -shared

all: talentedhack.so

talentedhack.so: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o talentedhack.so
cleanall: clean
	-rm talentedhack.so &>/dev/null;
clean:
	-rm $(OBJS) &>/dev/null;
	-rm dependencies/*.d &>/dev/null;
	
-include $(addprefix  dependencies/, $(OBJS:.o=.d))

%.o:%.c
	$(CC) -MMD -MP -c $(CFLAGS) $*.c -o $*.o; \
	mv $*.d dependencies/
install:
	echo "Copying this directory to ~/.lv2 ..."
	cp -r ../`basename \`pwd\`` ~/.lv2/
	
tarballs: talentedhack.so
	cd ..; rm talentedhack_source.tar.gz; tar -czvf talentedhack_source.tar.gz talentedhack.lv2/*.c talentedhack.lv2/*.h talentedhack.lv2/dependencies talentedhack.lv2/*.ttl talentedhack.lv2/Makefile;
	cd ..; rm talentedhack_linux_x86.tar.gz; tar -czvf talentedhack_linux_x86.tar.gz talentedhack.lv2/*.ttl talentedhack.lv2/talentedhack.so;

.PHONY : clean cleanall install tarballs
