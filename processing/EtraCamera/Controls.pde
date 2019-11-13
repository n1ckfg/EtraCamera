void keyPressed() {
  if(keyCode==33 || key==' ') {
    saveFrame("data/shot1_frame"+counter+".tga");
    counter++;
    img = createImage(sW, sH, RGB);
  }
  if(keyCode==34 || key=='c'||key=='C') {
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
