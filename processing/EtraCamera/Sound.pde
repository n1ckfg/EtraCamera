/**
 * This is a sine wave oscillator. The method .play() starts the oscillator. There
 * are several setter functions for configuring the oscillator, such as .amp(),
 * .freq(), .pan() and .add(). If you want to set all of them at the same time you can
 * use .set(float freq, float amp, float add, float pan)
 */

import processing.sound.*;

int numSines = 3;
SinOsc[] sines = new SinOsc[numSines];

float freq = 600;

void setupSound() {
  // create and start the sine oscillator.
  for (int i=0; i<sines.length; i++) {
    sines[i] = new SinOsc(this);
    sines[i].freq(freq+(i*2));
  }
}



void updateSound() {
  float time = sin((float)millis() / 100.0);
  time = map(time, -1.0, 1.0, 0.6, 0.8);
  //println(time);
  for (int i=0; i<sines.length; i++) {
    sines[i].amp(time);
  }
}
