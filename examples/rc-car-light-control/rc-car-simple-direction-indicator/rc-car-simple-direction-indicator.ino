
#include "RCServoDecoder.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define DEBUG_PRINT 1

const byte pinSteeringServoInput = 4;

/* direction light pins (non PWM) */
const byte pinLedBlinkFrontLeft = A0;
const byte pinLedBlinkFrontRight = A1;

/* blinking interval in milli seconds */
const unsigned long BlinkIntervalMs = 400;

const RCServoDecoder::Value BlinkThreshold = 10;

const RCServoDecoder::TimeDelta SystemMinMs =  538;
const RCServoDecoder::TimeDelta SystemMaxMs = 2375;

RCServoDecoder steeringChannel(pinSteeringServoInput,
                               SystemMinMs,
                               SystemMaxMs,
                               -100,
                               100);

void setup()
{
#if DEBUG_PRINT
  Serial.begin(9600);
#endif

  steeringChannel.begin();

  /* direction light pins (non PWM) */
  pinMode(pinLedBlinkFrontLeft, OUTPUT);
  pinMode(pinLedBlinkFrontRight, OUTPUT);
}

void loop()
{
  static unsigned long prevBlinkMs = millis();
  static byte blinkState = LOW;
  
  auto curSteering = steeringChannel.read();

#if DEBUG_PRINT
  Serial << " steering: " << curSteering << "\n";
  Serial << "\n";
#endif

  if (millis() - prevBlinkMs > BlinkIntervalMs) {
    prevBlinkMs = millis();
    blinkState = blinkState ? LOW : HIGH ;
  }

  /* handle left direction indicator */
  if (curSteering < -BlinkThreshold) {
    digitalWrite(pinLedBlinkFrontLeft, blinkState);
  } else {
    digitalWrite(pinLedBlinkFrontLeft, LOW);
  }
  
  /* handle right direction indicator */
  if (curSteering > BlinkThreshold) {
    digitalWrite(pinLedBlinkFrontRight, blinkState);
  } else {
    digitalWrite(pinLedBlinkFrontRight, LOW);
  }
}
