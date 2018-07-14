//////
//
//  OSC Repeater
//  The following program serves as a link between Wifi Enabled Arduinos 
//  and Unreal Engine. The communication could be done directly but this 
//  sketch helps map values, smooth steams and adjust them during run time.
//
//  Sebastian Morales- July 2018, IAAC Global Summer School NYC
//
/////


import oscP5.*;
import netP5.*;

import controlP5.*;    
ControlP5 controlP5;

OscP5 oscP5;
NetAddress toVr;
NetAddress toArduinoGas;
NetAddress toArduinoProx;

float[] average=new float[20]; 

float xoff = 0.0;
float value=0;
float gasMax=700;
float gasMin=500;
float proxMax=180;
float proxMin=0;
float max_threshold=700;
float min_threshold=500;
boolean on=true;
boolean gas=false;

void setup() {
  size(400, 400);
  //there are two possible sensors with different limmits but only one runs at the time.
  if (gas) {
    gui(gasMax, gasMin);
  } else {
    gui(proxMax, proxMin);
  }
  //init clear array for smoothing opperation
  for (int i=0; i<average.length; i++) {
    average[i]=0.0;
  }

  // Init OSC Settings
  oscP5 = new OscP5(this, 8080);
  toVr = new NetAddress("10.105.43.222", 8000);

  toArduinoGas = new NetAddress("172.16.10.180", 8000);
  //toArduinoProx = new NetAddress("172.16.10.224", 8000);
}

void draw() {
  background(204);
  drawGui();
  //in case we want to send noise
  //xoff = xoff + .01;
  //float n = noise(xoff);
  stroke(3);
  line(average[0]*width, 0, average[0]*width, height);
  smoothSensor(value);
}


void smoothSensor(float val) {
  value=val;//save var for later
  int max=2;
  //map val to limits
  val=map(val, min_threshold, max_threshold, 0, max);
  
  //trim val in case it goes over limits
  if (val>max)val=max;
  if (val<0) val=0;
  
  //We need to push values in average array one location. First we make a temporary array 
  //to save the values
  float[] temp=new float[average.length]; 
  for (int i=1; i<temp.length-1; i++) {
    temp[i]=average[i-1];
  }
  //the first point is now the value we just recieved
  average[0]=val;
  float sum=val;
  
  // We sum all the values to later get the average
  for (int i=1; i<average.length; i++) {
    average[i]=temp[i];
    sum+=average[i];
  }
  //the new smooth value is the average
  val=((sum/average.length));
  // Save it into the array
  average[0]=val;
  //send over OSC
  repeteOSC(val, "/sensor1", toVr);
}


//This function will take a value, a message name and an OSC Address.
void repeteOSC(float val, String msgName, NetAddress address) {
  OscMessage sensor1 = new OscMessage(msgName);
  sensor1.add(val); /* add an int to the osc message */
  oscP5.send(sensor1, address);
  println("Sending data: "+val +" to: "+msgName);
}


///PARSE OSC Events
void oscEvent(OscMessage theOscMessage) {
  if (theOscMessage.checkAddrPattern("/toVrGas")==true) {
    if (theOscMessage.checkTypetag("f")) {
      float v = theOscMessage.get(0).floatValue();
      smoothSensor(v);
      return;
    }
  }

  if (theOscMessage.checkAddrPattern("/toVr")==true) {
    if (theOscMessage.checkTypetag("f")) {
      float v = theOscMessage.get(0).floatValue();
      //smoothSensor(v);
      //repeteOSC(thirdValue,"/sensor1", toVr);
      return;
    }
  }

  if (theOscMessage.checkAddrPattern("/toArduinoLED")==true) {
    if (theOscMessage.checkTypetag("f")) {
      float v1 = theOscMessage.get(0).floatValue();
      repeteOSC(v1, "/toArduino", toArduinoGas);
      return;
    }
  }
  if (theOscMessage.checkAddrPattern("/toArduino")==true) {
    if (theOscMessage.checkTypetag("f")) {
      float v1 = theOscMessage.get(0).floatValue();
      repeteOSC(v1, "/toArduino", toArduinoProx);
      return;
    }
  }
}