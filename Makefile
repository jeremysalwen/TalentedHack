OBJS = circular_buffer.o  midi.o fft.o pitch_detector.o formant_corrector.o pitch_shifter.o lfo.o quantizer.o talentedhack.o pitch_smoother.o
DEBUG =-g
CFLAGS = --std=c99 -Wall -fPIC `pkg-config --cflags fftw3f` -O3 $(DEBUG)
LDFLAGS = $(DEBUG) -shared
BUNDLE=talentedhack.lv2

all: talentedhack.so

talentedhack.so: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) `pkg-config --libs fftw3f fftw3` -o talentedhack.so
cleanall: clean
	-rm talentedhack.so &>/dev/null;
clean:
	-rm $(OBJS) &>/dev/null;
	-rm dependencies/*.d &>/dev/null;
	
-include $(addprefix  dependencies/, $(OBJS:.o=.d))

%.o:%.c
	$(CC) -MMD -MP -c $(CFLAGS) $*.c -o $*.o; \
	mkdir -p dependencies
	mv $*.d dependencies/
install: all
	echo "Installing to ~/.lv2 ..."
	install -d ~/.lv2/$(BUNDLE)
	install -m755 talentedhack.so ~/.lv2/$(BUNDLE)
	install -m644 manifest.ttl talentedhack.ttl ~/.lv2/$(BUNDLE)
	
tarballs: talentedhack.so
	cd ..; rm talentedhack_source.tar.gz; tar -czvf talentedhack_source.tar.gz talentedhack.lv2/*.c talentedhack.lv2/*.h talentedhack.lv2/dependencies/*.d talentedhack.lv2/*.ttl talentedhack.lv2/Makefile;
	cd ..; rm talentedhack_linux_x86.tar.gz; tar -czvf talentedhack_linux_x86.tar.gz talentedhack.lv2/*.ttl talentedhack.lv2/talentedhack.so;

.PHONY : clean cleanall install tarballs
