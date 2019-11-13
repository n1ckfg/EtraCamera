import processing.video.*;

Capture cap;
boolean newFrame = false;

void setupCap(int w, int h) {
  cap = new Capture(this, w, h);
  cap.start();  
}

void updateCap() {
  if (cap.available()) {
    cap.read();
    //cap.loadPixels();
    newFrame = true;
  } else {
    newFrame = false;
  }
}
