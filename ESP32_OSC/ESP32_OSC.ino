//////
//  OSC Example for ESP32, By Luke Woodbury- 9th Jan 2018
//  Modified by Sebastian Morales to include MQ Gas Sensor
//  and light up an EL Wire
//
//  IAAC Global Summer School NYC
//////


//Wifi and OSC stuff
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

//WIFI Settings
char ssid[] = "*******"; //network SSID (name)
char pass[] = "*******";    //password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(10, 105, 45, 236);     // remote IP of your computer
//const IPAddress outIp(192, 168, 0, 7);     // remote IP of your computer
const unsigned int outPort = 8080;          // remote port to receive OSC
const unsigned int localPort = 8000;        // local port to listen for OSC packets (actually not used for sending)

#define LEDpin 13

const int elWire1 = 12;
long int now = 0;
int interval = 250;


void setup() {
  
  // Set up the PINS
  pinMode(elWire1, OUTPUT);
  
  //Set Serial Communication 
  Serial.begin(115200);

  // Connect to WiFi network
  initWifi();
}

void loop() {
  checkOSCin();
  if (millis() - now > interval) {
    now = millis();
    checkGas();
  }
}


void sendOSC(float val) {
  OSCMessage msg("/toVrGas");
  msg.add(val);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // Send the bytes to the SLIP stream
  Udp.endPacket();  // Mark the end of the OSC Packet
  msg.empty();   // Free space occupied by message
  digitalWrite(LEDpin, HIGH);
  delay(200);
  digitalWrite(LEDpin, LOW);
  delay(200);
}


void checkOSCin() {
  OSCMessage msgIn;
  int size;
  if ( (size = Udp.parsePacket()) > 0) {
    //    Serial.println("here");
    while (size--)
      msgIn.fill(Udp.read());

    if (!msgIn.hasError()) {
      msgIn.dispatch("/toArduino", dispatchAddress);
    }
  }
}


void dispatchAddress(OSCMessage &msg) {
  // Even though we are classifying, wekinator sends outputs as floats
  if (msg.isFloat(0)) {
    float val = msg.getFloat(0);
    if (val == 1) {
      digitalWrite(elWire1, HIGH);
      Serial.println("Turning LIGH1 on");
    }
    if (val == 3) {
      digitalWrite(elWire1, LOW);
      Serial.println("Turning lights off");
    }
  }
}
