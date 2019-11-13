import processing.video.*;

Capture cap;
PImage frame;
boolean newFrame = false;

void setupCap(int w, int h) {
  cap = new Capture(this, w, h);
  frame = createImage(w, h, RGB);
  frame.loadPixels();
  cap.start();  
}

void updateCap() {
  if (cap.available()) {
    cap.read();
    frame = cap.copy();
    newFrame = true;
  } else {
    newFrame = false;
  }
}
