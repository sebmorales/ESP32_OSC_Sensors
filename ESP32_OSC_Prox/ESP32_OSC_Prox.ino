//////
//  OSC Example for ESP32, By Luke Woodbury- 9th Jan 2018
//  Modified by Sebastian Morales to include a Proximity
//  ultrasonic sensor and some vibration motors.
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
//const IPAddress outIp(192, 168, 0, 7);   
const unsigned int outPort = 8080;          // remote port to receive OSC
const unsigned int localPort = 8000;        // local port to listen for OSC packets (actually not used for sending)

#define LEDpin 13

//pause without dealy variables
long int now = 0;
int interval = 250;

// Set up sensors
int trigPin = 14;    //Trig - green Jumper
int echoPin = 12;    //Echo - yellow
long duration;
long cm;
long inches;

// Set up Vibration Motors
const int motor1 = 27;
const int motor2 = 26;
const int motor3 = 25;
const int motor4 = 32;



void setup() {

  // Set up motor PINS
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);

  //Set sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Set Serial Communication 
  Serial.begin(115200);

  // Connect to WiFi network
  initWifi()'

}

void loop() {
  if (millis() - now > interval) {
    now = millis();
    checkProx();
  }
}


void sendOSC(float val) {
  OSCMessage msg("/toVrGas");
  msg.add(val);

  Udp.beginPacket(outIp, outPort);
  msg.send(Udp); // Send the bytes to the SLIP stream
  Udp.endPacket();  // Mark the end of the OSC Packet
  msg.empty();   // Free space occupied by message
  //  digitalWrite(LEDpin, HIGH);
  delay(200);
  //  digitalWrite(LEDpin, LOW);
  delay(200);
}

