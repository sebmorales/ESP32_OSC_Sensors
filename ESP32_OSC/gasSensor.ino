int gasPin = A3;//SM

void checkGas() {
  float temp;
  temp = analogRead(gasPin);
  Serial.println(temp);
  sendOSC(temp);
  if (temp > 650) {
    digitalWrite(elWire1, HIGH);
  } else {
    digitalWrite(elWire1, LOW);
  }
}
