AY Synth
--------

This is a small PIC32-based AY-3-8912 MIDI synthesizer.

The concept is simple: have a PIC32 appear as a standard USB
MIDI device, and have that control an AY-3-8912 in such a way
that it performs the basic functions of a MIDI synthesizer.

The prototype was built using a chipKIT DP32 (PIC32MX250F128B)
prototyping board, although any USB-enabled PIC32MX that has a
REFCLK would do the job.

The AY-3-8912 is wired directly to the PIC32 (the PIC32 is run
at 3.3V and the AY-3-8912 at 5V) according to the `#define`
entries at the top of the accompanying sketch (you can change
them to be any pins you want, except the CLOCK pin which requires
the output of a 1.6MHz REFCLK).

At the moment there are three separate MIDI channels each with
a hard-coded instrument assigned. Eventually multiple instruments
will be available and program-change messages will be honoured
to select the appropriate instrument. 

Each channel can only play one note at a time.


