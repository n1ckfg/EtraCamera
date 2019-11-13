void keyPressed() {
  if(keyCode==33 || key==' ') {
    for (int i=0; i<sines.length; i++) {
      sines[i].play();
    }
    doCapture = true;
    showCam = false;
  }
  
  if(keyCode==34 || key=='s' || key=='S') {
    saveFrame("data/shot1_frame"+counter+".tga");
    counter++;
    img = createImage(sW, sH, RGB);
    showCam = true;
  }
  
  if (keyCode==TAB) {
    showImg = !showImg; 
  }
  
  if (keyCode==UP || keyCode==DOWN) {
    if (keyCode==UP) rThreshold++;
    if (rThreshold > 255) rThreshold = 255;
    if (keyCode==DOWN) rThreshold--;
    if (rThreshold < 0) rThreshold = 0;
    println(rThreshold);
   }
}

void keyReleased() {
    if(keyCode==33 || key==' ') {
      for (int i=0; i<sines.length; i++) {
        sines[i].stop();
      }
      
      doCapture = false;
    }
}

/*
void mousePressed() {
  for (int i=0; i<sines.length; i++) {
    sines[i].play();
  }
  
  doCapture = true;
  showCam = false;
}

void mouseReleased() {
  for (int i=0; i<sines.length; i++) {
    sines[i].stop();
  }
  
  doCapture = false;
}
*/
