
#include "RCServoDecoder.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#define DEBUG_PRINT 1

const byte pinSpeedServoInput = 2;
const byte pinSteeringServoInput = 4;
const byte pinSwitchServoInput = 7;

/* direction light pins (non PWM) */
const byte pinLedBlinkFrontLeft = A0;
const byte pinLedBlinkFrontRight = A1;
const byte pinLedBlinkBackLeft = A2;
const byte pinLedBlinkBackRight = A3;

/* brightness controlled light pins (PWM) */
const byte pinLedFrontWhite = 3;
const byte pinLedBackRed = 5;
const byte pinLedBackBrake = 6;
const byte pinLedBackWhite = 9;

/* brightness levels */
const byte HeadLightLevelDefault = 10;
const byte HeadLightLevelHigh = 255;
const byte BackLightLevelDefault = 20;
const byte BackLightLevelBrake = 255;
const byte BackLightLevelWhiteDefault = 75;

/* thresholds */
const RCServoDecoder::Value HeadLightThreshold = 70;
const RCServoDecoder::Value BlinkThreshold = 10;
const RCServoDecoder::Value BreakThreshold = -10;
const RCServoDecoder::Value ReverseThreshold = -70;
const RCServoDecoder::Value SwitchThreshold = 70;

/* blinking interval in milli seconds */
const unsigned long BlinkIntervalMs = 400;

const RCServoDecoder::TimeDelta SystemMinMs =  538;
const RCServoDecoder::TimeDelta SystemMaxMs = 2375;

RCServoDecoder speedChannel(pinSpeedServoInput,
                            SystemMinMs,
                            SystemMaxMs,
                            100,
                            -100);
RCServoDecoder steeringChannel(pinSteeringServoInput,
                               SystemMinMs,
                               SystemMaxMs,
                               -100,
                               100);
RCServoDecoder switchChannel(pinSwitchServoInput,
                             SystemMinMs,
                             SystemMaxMs,
                             -100,
                             100);

void setup()
{
#if DEBUG_PRINT
  Serial.begin(9600);
#endif

  speedChannel.begin();
  steeringChannel.begin();
  switchChannel.begin();


/* direction light pins (non PWM) */
pinMode(pinLedBlinkFrontLeft, OUTPUT);
pinMode(pinLedBlinkFrontRight, OUTPUT);
pinMode(pinLedBlinkBackLeft, OUTPUT);
pinMode(pinLedBlinkBackRight, OUTPUT);

/* brightness controlled light pins (PWM) */
pinMode(pinLedFrontWhite, OUTPUT);
pinMode(pinLedBackRed, OUTPUT);
pinMode(pinLedBackBrake, OUTPUT);
pinMode(pinLedBackWhite, OUTPUT);
  
}

void loop()
{
  static unsigned long prevBlinkMs = millis();
  static byte blinkState = LOW;
  
  auto curSpeed    = speedChannel.read();
  auto curSteering = steeringChannel.read();
  auto curSwitch   = switchChannel.read();

#if DEBUG_PRINT
  Serial << "    speed: " << curSpeed << "\n";
  Serial << " steering: " << curSteering << "\n";
  Serial << "   switch: " << curSwitch << "\n";
  Serial << "\n";
#endif

  if (millis() - prevBlinkMs > BlinkIntervalMs) {
    prevBlinkMs = millis();
    blinkState = blinkState ? LOW : HIGH ;
  }

  byte headLightLevel;
  byte backLightLevel;
  byte backLightLevelBreak;
  byte backLightLevelWhite;
  
  if (curSwitch > SwitchThreshold) {
  /* hazard lights enabled */
    digitalWrite(pinLedBlinkFrontLeft, blinkState);
    digitalWrite(pinLedBlinkBackLeft, blinkState);
    digitalWrite(pinLedBlinkFrontRight, blinkState);
    digitalWrite(pinLedBlinkBackRight, blinkState);

    headLightLevel = 0;
    backLightLevel = BackLightLevelDefault;
    backLightLevelBreak = 0;
    backLightLevelWhite = 0;
  } else {
    /* regular (non-hazard) mode */
    
    /* handle left direction indicator */
    if (curSteering < -BlinkThreshold) {
      digitalWrite(pinLedBlinkFrontLeft, blinkState);
      digitalWrite(pinLedBlinkBackLeft, blinkState);
    } else {
      digitalWrite(pinLedBlinkFrontLeft, LOW);
      digitalWrite(pinLedBlinkBackLeft, LOW);
    }
    /* handle right direction indicator */
    if (curSteering > BlinkThreshold) {
      digitalWrite(pinLedBlinkFrontRight, blinkState);
      digitalWrite(pinLedBlinkBackRight, blinkState);
    } else {
      digitalWrite(pinLedBlinkFrontRight, LOW);
      digitalWrite(pinLedBlinkBackRight, LOW);
    }

    headLightLevel = HeadLightLevelDefault;
    backLightLevel = BackLightLevelDefault;
    backLightLevelBreak = BackLightLevelDefault;
    backLightLevelWhite = 0;

    if (curSpeed < ReverseThreshold) {
      /* driving backward*/
      backLightLevelWhite = BackLightLevelWhiteDefault;
    } else {
      /* driving forward */

      /* detect brake */
      if (curSpeed < BreakThreshold) {
        backLightLevel = BackLightLevelBrake;
        backLightLevelBreak = BackLightLevelBrake;
      }

      /* detect full power */
      if (curSpeed > HeadLightThreshold) {
        headLightLevel = HeadLightLevelHigh;
      }
    }
  }

  /* write brightnesses */
  analogWrite(pinLedFrontWhite, headLightLevel);
  analogWrite(pinLedBackRed, backLightLevel);
  analogWrite(pinLedBackBrake, backLightLevelBreak);
  analogWrite(pinLedBackWhite, backLightLevelWhite);
}
