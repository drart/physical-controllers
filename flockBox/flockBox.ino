#include <math.h>
#include <OSCMessage.h>
#include <SLIPEncodedUSBSerial.h>

const int DELAY_RATE = 1;
const int NUM_POTS = 7;
const int NUM_BUTTONS = 2;
const int RESOLUTION = 8;
const float MAX_VAL = (float)pow(2, RESOLUTION) - 1.0;
const String ADDR_PREFIX = "/knobs/";
const String ADDR_BUTTON_PREFIX = "/buttons/";

int potPins[NUM_POTS] = {0, 1, 2, 3};
byte prevVals[NUM_POTS] = {0, 0, 0, 0};

int buttonPins[NUM_BUTTONS] = {2, 3};
byte prevButtonVals[NUM_BUTTONS] = {0, 0, 0, 0};

SLIPEncodedUSBSerial SLIPSerial(Serial);

void setup() {
  Serial.begin(115200); // Rate is ignored for USB devices such as the Teensy.
  analogReadResolution(RESOLUTION);
  analogReadAveraging(32);
  analogRead(0);
}

void loop() {

  // read analog inputs
  for (int i = 0; i < NUM_POTS; i++) {
    int potPin = potPins[i];
    byte prevVal = prevVals[i];
    byte val = analogRead(potPin);
    if (val != prevVal) {
      float scaled = (float)val / MAX_VAL;
      handleValueOSC(potPin, scaled);
    }
    prevVals[i] = val;

    delay(DELAY_RATE);
  }

  // read digital inputs
  for (int i = 0; i < NUM_POTS; i++) {
    int potPin = buttonPins[i];
    byte prevButtonVal = prevButtonVals[i];
    byte val = digitalRead(potPin);
    if (val != prevButtonVal) {
      handleButtonOSC(potPin, val);
    }
    prevVals[i] = val;

    delay(DELAY_RATE);
  }
}

void handleButtonOSC(int digPin, float val) {
  String address = String(ADDR_BUTTON_PREFIX + potPin);
  int addressLength = address.length() + 1;
  char addressBuffer[addressLength];

  address.toCharArray(addressBuffer, addressLength);
  OSCMessage msg(addressBuffer);
  msg.add(val);

  msg.send(SLIPSerial);
  SLIPSerial.endPacket();
  msg.empty();
}

void handleValueOSC(int potPin, float val) {
  String address = String(ADDR_PREFIX + potPin);
  int addressLength = address.length() + 1;
  char addressBuffer[addressLength];

  address.toCharArray(addressBuffer, addressLength);
  OSCMessage msg(addressBuffer);
  msg.add(val);

  msg.send(SLIPSerial);
  SLIPSerial.endPacket();
  msg.empty();
}

void handleValueDebug(int potPin, float val) {
  Serial.print("Knob ");
  Serial.print(potPin);
  Serial.print(": ");
  Serial.println(val);
}
