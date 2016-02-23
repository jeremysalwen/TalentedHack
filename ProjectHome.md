This is an LV2 port of Tom Baran's Autotalent (http://web.mit.edu/tbaran/www/autotalent.html), with added features and improved performance.

This plugin should work basically the same as Autotalent except:

  * It is an LV2 plugin instead of an LADSPA plugin
  * It provides MIDI output of the pitch
  * It accepts MIDI input
  * It separates the pull to semitone and snap to scale functionality
  * It uses FFTW for the DFT routines, greatly improving performance.
  * Various minor performance tweaks
  * Slightly more accurate pitch detection (Uses MPM method instead of straight autocorrelation)
  * It is greatly refactored (broken into methods and structures, variables named)
  * The formant corrector causes artifacts not present in the original (I'm not sure how to fix, as it's the only part of the original I don't understand)

Something I've noticed when testing it is that it has difficulty detecting low notes.  I think this might be remedied by increasing the buffer size of the pitch detector.  Anyway, the same problem is present in the original.


### To compile ###
Make sure you have the header files of fftw3 installed, and simply run "make" inside the source directory, and that directory should function as an LV2 plugin. Usually if you put it inside the .lv2/ folder in your home directory (or /usr/lib/lv2/, but be careful installing stuff manually into system folders), most programs should find it. I'm not really sure how portable it is, but it works for me on 32 bit linux with gcc.

### Future features ###
  * Fixed formant corrector
  * Better note onset detection (aubio?)
  * Transient/Steady State filtering (aubio)

### Version 1 ###
Initial release.  We'll see how this goes.

### Version 1.5 ###
Updated the pitch detection code to use the peak picking algorithm of Tartini (MPM)

### Version 1.6 ###
Implemented the LFO

### Version 1.7 ###
Fixed unnecessary pitch jumping.

### Version 1.8 ###
Added Pitch Smoothing

#### Version 1.81 ####
Made Midi ports optional thanks to Gabriel Beddingfield

#### Version 1.82 ####
Fixed linking issue with GCC 4.4.
Fixed Makefile to include dependencies folder in tarballs.

#### Version 1.83 ####
Cleaned up the branding so it won't be confused with Autotalent any more.

#### Version 1.84 ####
Fixed crash which occurred when using high sampling rates.

#### Version 1.85 ####
Made the peak picking code more efficient.
### Contact ###
If you have any questions, please contact me, at jeremysalwen@gmail.com