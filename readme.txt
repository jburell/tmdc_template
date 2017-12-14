This is a demo template aimed at ASCII demo making.
It was specially made for TMDC, as a tool for newcomers.
The framework handles the most tricky and boring tasks
of demo making: linking external libs (fmod+caca) and
getting sometihng *working*.

The music is a module file (.IT) made with ModplugTracker.
The IT contains Zxy events on which the framework relies to
actually launch scenes and add effects.

The graphics are RAW image made with photoshop (save as .RAW,
interleave channels) as 32bit (with a mandatory alpha channel)
The current code recognizes the size of the image by its name,
for example mypic_320x240.raw (width 320, height 240).
When opening them, Photoshop rarely guess the format right.
The number of channels is 4 and the w/h should be those
in the filename.

Plz note that this framework has been used 
in several winning demos. This is no bullshit!

I hope it's useful and that you'll enjoy using it!
-BarZoule/ND^Bawlz

_____________________________________________________________________________


CREDZ:
 Most of the code and all assets from this template, made by BarZoule/ND^Bawlz
 JOES11.IT shamelessly ripped and customized from Jesus On Es
 Libcaca by Sam Hocevar
 Possibly additional misc code by Guybrush, Hardliner and Blacklight

 Most of the code was used (and made for)
  Signal.2.Noize (tmdc8),
  Signal to Droids (tmdc9),
  Session One (tmdc10),
  ClockWerk (tmdc10),
  as well as some unreleased GP2X prods.
