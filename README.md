fussgoalie
==========

Goal counter and speed sensor for fussball tables

Built using an Arduino Mega 2560. In each goal, there are two small lasers (red laser pointer-style), and two photodiode 
sensors. The sensors are powered, and have a TTL output. When a ball passes through the goal, it will hit the beam and cause 
the output from that sensor to change (go high). By measuring the time between the front and back triggers and knowing the 
distance between them (which is about 3cm), you can calculate the speed.

Photo sensors:
http://dx.com/p/photosensitive-diode-light-sensor-module-140676

Lasers:
http://dx.com/p/6mm-5mw-red-laser-module-3-5-4-5v-13378

Arduino pins 18-21 are used for input. These stay clear of any regular-sized shields, and have interrupts on state changes.

John Stäck <john@stack.se>
