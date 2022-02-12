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
  * The formant corrector is broken :(.

Something I've noticed when testing it is that it has difficulty detecting low notes.  I think this might be remedied by increasing the buffer size of the pitch detector.  Anyway, the same problem is present in the original.


### To compile ###
Make sure you have the header files of fftw3 installed, and simply run "make" inside the source directory, and that directory should function as an LV2 plugin. Usually if you put it inside the .lv2/ folder in your home directory (or /usr/lib/lv2/, but be careful installing stuff manually into system folders), most programs should find it. I'm not really sure how portable it is, but it works for me on 32 bit linux with gcc.


### Contact ###
If you have any questions, please contact me, at jeremysalwen@gmail.com