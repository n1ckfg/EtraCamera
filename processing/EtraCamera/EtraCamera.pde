import processing.video.*;

//red laser scanning
float rThreshold = 0;
float rScale = 0.1;
boolean showImg = true;
boolean useSound = true;

int sW = 1280;
int sH = 720;
int fps = 30;

PImage img;
int counter=1;
float avgBright = 0;
float pAvgBright = 0;

Settings settings;

void setup() {
  size(50, 50);
  settings = new Settings("settings.txt");
  surface.setSize(sW, sH);
  frameRate(fps);
  
  setupCap(sW, sH);
 
  img = createImage(sW,sH,RGB);
  if (useSound) setupSound();
}

void draw() {
  if (useSound) updateSound();
  
  if(cap.available()) cap.read();
  if(showImg) {
  img.loadPixels();
  pAvgBright=avgBright;
  avgBright=0;
  for (int i=0; i<img.width * img.height;i++) {
    try {
      float r = red(cap.pixels[i]);
      avgBright+=r;
      if (r >= rThreshold) {
        r*=rScale;
        r += red(img.pixels[i]);
        if (r > 255) r=255;
        if (r < 0) r=0;
        img.pixels[i] = color(r);
      }
    } catch (Exception e) { } 
  }
  img.updatePixels();
  
  //if(avgBright>pAvgBright+2) {
    //saveFrame("data/shot1_frame"+counter+".tga");
    //counter++;
  //}
  }
  if (showImg) {
    image(img, 0, 0);
  } else {
    image(cap, 0, 0);
  }
  // The following does the same, and is faster when just drawing the image
  // without any additional resizing, transformations, or tint.
  //set(0, 0, cap);
  //println(frameRate);
}

float maxDistance = -1.0;

void getMaxDistance() {
  for (int i=1; i<256; i++) {
    maxDistance += 1.0/i;
  }
}

float getDistance(float val) {
  if (maxDistance < 0) getMaxDistance();
  
  float returns = 0.0;

  for (int i=1; i<val+1; i++) {
    returns += 1.0/i;
  }
  
  return abs(maxDistance - returns) + 1.0;
}
