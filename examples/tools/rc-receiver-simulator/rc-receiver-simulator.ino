/** rc-receiver-simulator
 * 
 * This sketch simulates a RC receiver with three output channels,
 * A, B and C.
 *
 * A and B are linear channels which generate a signal from 0 to 180
 * degree, C is a switch which generates 0 degree when not pressed
 * and 180 degree when pressed.
 *
 * 
 * 
 * Two analog inputs are sampled to get 
 */
#include <Servo.h>

/** The pin of the analog input A.
 * The input expects a potentiometer generated voltage ranging from 0 to 5V
 */
const byte pinAnalogA = A0;
/** The pin of the analog input B.
 * The input expects a potentiometer generated voltage ranging from 0 to 5V
 */
const byte pinAnalogB = A1;
/** The pin of the digital input C.
 * Expects a switch connected to ground, the internal pullup will be activated.
 */
const byte pinDigitalC = 2;

/** The output pin of the simulated servo channel A.
 * Connect this to your device under test (DUT).
 */
const byte pinServoA = 3;
/** The output pin of the simulated servo channel B.
 * Connect this to your device under test (DUT).
 */
const byte pinServoB = 4;
/** The output pin of the simulated servo channel C.
 * Connect this to your device under test (DUT).
 */
const byte pinServoC = 5;

Servo sA;
Servo sB;
Servo sC;

void setup()
{
  Serial.begin(9600);
  
  pinMode(pinDigitalC, INPUT_PULLUP);
  
  sA.attach(pinServoA);
  sB.attach(pinServoB);
  sC.attach(pinServoC);
}

void loop()
{
  auto a = analogRead(pinAnalogA);
  a = map(a, 0, 1023, 0, 180);
  sA.write(a);

  auto b = analogRead(pinAnalogB);
  b = map(b, 0, 1023, 0, 180);
  sB.write(b);

  auto sw = digitalRead(pinDigitalC);
  sC.write(sw == LOW ? 180 : 0);

  Serial.print("sw: ");
  Serial.println(sw);
  Serial.print("a: ");
  Serial.println(a);
  Serial.print("b: ");
  Serial.println(b);
  Serial.println("");

  delay(200);
}
