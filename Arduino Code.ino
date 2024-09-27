Arduino Code

#include <IRremote.h>
#include <dht.h>

const int eyeBlinkPin = A0;
const int blinkThreshold = 500;
const int minBlinkDuration = 100;
const int maxBlinkDuration = 500;
const int serialBaudRate = 9600;

int previousEyeBlinkValue = 0;
unsigned long lastBlinkTime = 0;
int continuousBlinks = 0;

const int irEmitterPin = 8;

IRsend irsend(irEmitterPin);

void setup() {
  Serial.begin(serialBaudRate);
  pinMode(irEmitterPin, OUTPUT);
  irsend.enableIR();

}

void loop() {
  int eyeBlinkValue = analogRead(eyeBlinkPin);

  int smoothedValue = (previousEyeBlinkValue + eyeBlinkValue) / 2;
  previousEyeBlinkValue = eyeBlinkValue;

  if (abs(smoothedValue - previousEyeBlinkValue) > 100) {
    unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime > 500) {
      if (smoothedValue < blinkThreshold) {
        lastBlinkTime = currentTime;
      } else if (smoothedValue > blinkThreshold && currentTime - lastBlinkTime > minBlinkDuration) {
        int blinkDuration = currentTime - lastBlinkTime;
        if (blinkDuration >= minBlinkDuration && blinkDuration <= maxBlinkDuration) {
          continuousBlinks++;

          if (continuousBlinks > 10) { 
            Serial.println("Camera Activated!");
            continuousBlinks = 0;
          }
        } else {
          continuousBlinks = 0;
        }
      }
    }
  }

  if (Serial.available() > 0) {
    String objectName = Serial.readStringUntil('\n');
    objectName.trim();

    if (objectName == "microwave") {
      if (continuousBlinks % 2 == 1) {
        irsend.sendSamsung(0xABCD);
        delay(1000);
        Serial.println("Microwave turned on!");
      } else if (continuousBlinks % 2 == 0) {
        irsend.sendSamsung(0xDCBA);
        delay(1000);
        Serial.println("Microwave turned off!");
      }
      continuousBlinks = 0;
    } else if (objectName == "television") {
      if (continuousBlinks % 2 == 1) {
        irsend.sendSamsung(0xA847);
        delay(1000);
        Serial.println("Television turned on!");
      } else if (continuousBlinks % 2 == 0) {
        irsend.sendSamsung(0XCODE);
        delay(1000);
        Serial.println("Television turned off!");
      }
      continuousBlinks = 0;
    }
  }
}

Arduino code description

The Arduino code focuses on detecting eye blinks using an analog sensor connected to pin A0. It implements a mechanism to avoid double-counting blinks due to sensor . It counts consecutive blinks and resets the counter if the blink duration falls outside a specific range. Once a threshold number of continuous blinks is reached, it sends signals to the Python program via serial communication that it's ready to receive an object name, After successful identification of object by the python program, Arduino code will capture the object name in the serial monitor, Based on the received object name the Arduino will activate the IR sensor to change the status of the object(light, television, microwoven and other).