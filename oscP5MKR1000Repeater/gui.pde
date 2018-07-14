void gui(float max,float min) { 
  controlP5 = new ControlP5(this);
  controlP5.addSlider("max_threshold", min/2, max*1.3, max, 20, 10, 100, 10);
  controlP5.addSlider("min_threshold", 0, min*1.3+10, min, 20, 30, 100, 10);
}


void drawGui() {
  fill(7);
  noStroke();
  textSize(14);
  text("RAW Value:"+ value, width-130, 20); 
}