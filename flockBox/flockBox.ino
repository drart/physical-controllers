
#include <math.h>
#include <OSCMessage.h>
#include <SLIPEncodedUSBSerial.h>

const float LOG001 = log(0.001);

const int delayRate = 1;
const int numPots = 4;
const int maxUnscaledValue = 4095;
const String addrPrefix = "/pots/";

int potPins[numPots] = {0, 1, 2, 3};
int prevVals[numPots] = {0.0, 0.0, 0.0, 0.0};

SLIPEncodedUSBSerial SLIPSerial(Serial);

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < numPots; i++) {
    analogReadResolution(12);
    analogReadAveraging(32);
    
    int potPin = potPins[i];
    int prevVal = prevVals[i];
    int val = analogRead(potPin);
    double scaled;
    
    if (val != prevVal) {
      //scaled = scale(val);
      handleValueOSC(potPin, val);
    }
    
    prevVals[i] = val;    
  }
  
  delay(delayRate);
}

double scale(int val) {
  return (double) (val / maxUnscaledValue);
}

void handleValueOSC(int potPin, int val) {
  String address = String(addrPrefix + potPin);
  int addressLength = address.length() + 1;
  char addressBuffer[addressLength];
  
  address.toCharArray(addressBuffer, addressLength);
  OSCMessage msg(addressBuffer);
  msg.add(val);
  
  msg.send(SLIPSerial);
  SLIPSerial.endPacket();
  msg.empty();
}

void handleValueDebug(int potPin, int val) {
  Serial.print("Pot ");
  Serial.print(potPin);
  Serial.print(": ");
  Serial.println(val);
}


