import processing.video.*;

//red laser scanning
float rThreshold = 0;
float rScale = 0.1;
boolean showImg=true;

int sW = 1280;
int sH = 720;
int fps = 30;
Capture cam;
PImage img;
int counter=1;
float avgBright = 0;
float pAvgBright = 0;

void setup(){
  size(sW,sH);
  frameRate(fps);
  // The name of the capture device is dependent on
  // the cameras that are currently attached to your 
  // computer. To get a list of the 
  // choices, uncomment the following line 
  // println(Capture.list());
  
  // To select the camera, replace "Camera Name" 
  // in the next line with one from Capture.list()
  // cam = new Capture(this, width, height, "Camera Name", 30);
  
  // This code will try to use the camera last used
  initCam();
  img = createImage(sW,sH,RGB);
}

void draw() {
  if(cam.available()) cam.read();
  if(showImg){
  img.loadPixels();
  pAvgBright=avgBright;
  avgBright=0;
  for(int i=0; i<img.width * img.height;i++){
    try{
    float r = red(cam.pixels[i]);
    avgBright+=r;
    if(r >= rThreshold){
      r*=rScale;
      r += red(img.pixels[i]);
      if(r > 255) r=255;
      if(r < 0) r=0;
      img.pixels[i] = color(r);
    }
    }catch(Exception e){ } 
  }
  img.updatePixels();
  if(avgBright>pAvgBright+2){
    saveFrame("data/shot1_frame"+counter+".tga");
    counter++;
  }
  }
  if(showImg){
    image(img, 0, 0);
  }else{
    image(cam, 0, 0);
  }
  // The following does the same, and is faster when just drawing the image
  // without any additional resizing, transformations, or tint.
  //set(0, 0, cam);
  //println(frameRate);
}

void keyPressed(){
  if(keyCode==33||key==' '){
    img = createImage(sW,sH,RGB);
    counter=1;
  }
  if(keyCode==34||key=='c'||key=='C'){
    showImg = !showImg; 
  }
  if(keyCode==UP||keyCode==DOWN){
    if(keyCode==UP) rThreshold++;
    if(rThreshold > 255) rThreshold = 255;
    if(keyCode==DOWN) rThreshold--;
    if(rThreshold < 0) rThreshold = 0;
    println(rThreshold);
   }
}

void initCam(){
   String[] cameras = Capture.list();
    
    if (cameras.length == 0) {
      println("There are no cameras available for capture.");
      exit();
    } else { 
      println("Available cameras:");
      for (int i = 0; i < cameras.length; i++) {
        println(cameras[i]);
      }
  }
    // The camera can be initialized directly using an 
    // element from the array returned by list():
    cam = new Capture(this, cameras[0]);
    cam.start();     
} 
