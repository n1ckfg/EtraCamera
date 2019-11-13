import peasy.PeasyCam;

PeasyCam cam;

//red laser scanning
float rThreshold = 0;
float rScale = 0.1;
boolean useSound = true;
boolean showImg = false;
boolean showCam = true;

int sW = 1280;
int sH = 720;
int fps = 30;

PImage img;
PShape shp;
int scanInterval = 5;
int counter=1;
float avgBright = 0;
float pAvgBright = 0;

boolean doCapture = false;
boolean firstRun = true;

Settings settings;

void setup() {
  size(1280, 720, P3D);
  cam = new PeasyCam(this, width/2, height/2, 400, 50);

  settings = new Settings("settings.txt");
  frameRate(fps);
  
  img = createImage(sW,sH,RGB);
  img.loadPixels();
  shp = createShape();
  shp.beginShape(POINTS);
  shp.stroke(255, 63);
  shp.strokeWeight(4);
  for (int i=0; i<int(img.pixels.length/scanInterval); i++) {
    shp.vertex(0,0,0);
  }
  shp.endShape();
  
  if (useSound) setupSound();
}

void draw() {
  background(0);
  
  if (firstRun) {
    setupCap(sW, sH);
    firstRun = false;
  } else {
    updateCap();
 
    if (!showCam) {
      if (doCapture) {
      if (useSound) updateSound();
      
      if (newFrame) {
        int shapeCounter = 0;
        for (int y=0; y<img.height; y+=scanInterval) {
          for (int x=0; x<img.width; x+=scanInterval) {
            int loc = x + y * img.width;
            float r = red(frame.pixels[loc]);
            if (r >= rThreshold) {
              r*=rScale;
              r += red(img.pixels[loc]);
              if (r > 255) r=255;
              if (r < 0) r=0;
              img.pixels[loc] = color(r);
              shp.setVertex(shapeCounter, new PVector(x, y, getDistance(r) * -500.0));
            }
            
            shapeCounter++;
          }
        }
        img.updatePixels();
        }
      }
      
      if (showImg) {
        image(img, 0, 0); 
      } else {
        shape(shp);
      }
    } else {
      image(frame, 0, 0);
    }
  }
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
