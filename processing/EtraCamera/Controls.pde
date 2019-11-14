void startScan() {
  for (int i=0; i<sines.length; i++) {
    sines[i].play();
  }
  doCapture = true;
  showCam = false;
}

void stopScan() {
  for (int i=0; i<sines.length; i++) {
    sines[i].stop();
  }
  
  doCapture = false;
}

void keyReleased() {
  println(keyCode);
  
  if(keyCode==16 || key==' ') {
    if (!doCapture) {
      startScan();
    } else {
      stopScan();
    }
  }
  
  if(keyCode==11 || key=='s' || key=='S') {
    stopScan();
    
    String url = "data/frame"+counter;
    saveFrame(url + "view.tga");
    img.save(url + ".tga");
    
    stringList = new ArrayList<String>(); 
    for (int i=0; i<pts.length; i++) {
      try {
        PVector pt = pts[i];
        stringList.add(pt.x + ", " + pt.y + ", " + pt.z);
      } catch (Exception e) { }
    }
    
    String[] stringArray = new String[stringList.size()];
    saveStrings(url + ".xyz", stringList.toArray(stringArray));
    
    counter++;
    img = createImage(sW, sH, RGB);
    showCam = true;
  }
}

void keyPressed() {
  if (keyCode==TAB) {
    showImg = !showImg; 
  }
  
  if (keyCode==UP) {
    rThreshold++;
    rThreshold = constrain(rThreshold, 0, 255);
  } else if (keyCode==DOWN) {
    rThreshold--;
    rThreshold = constrain(rThreshold, 0, 255);
  }
}
