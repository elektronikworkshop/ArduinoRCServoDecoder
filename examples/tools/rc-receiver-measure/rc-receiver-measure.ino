
/** rc-receiver-measure
 * 
 * This sketch measures the pulse lenghts in microseconds of a RC receiver
 * channel.
 * 
 * Use this sketch to determine the minimum and maximum pulse durations. Adjust
 * the testPinNumber to the pin number where you have your RC receiver channel
 * attached. Or plug your receiver channel into the corresponding default pin
 * which is 2 as can be seen below.
 * 
 * Open the Serial Monitor within your Arduino-IDE and set it to 9600 bauds.
 * 
 * You can then use these minimum and maximum pulse in your actual sketch when
 * initializing the RCServoDecoder-objects.
 * 
 */

#include <RCServoDecoder.h>

/** Modify this to the pin number to which you have your RC receiver channel
 * attached to or attach your receiver channel to this pin.
 */
const byte testPinNumber = 2;

RCServoDecoder decoder(testPinNumber);

void setup()
{
  Serial.begin(9600);
  Serial.println("");

  decoder.begin();
  
  Serial.print("listening to pin ");
  Serial.println(testPinNumber);
  Serial.println("move lever/switch to position(s) you'd like to measure");
}

void loop()
{
  Serial.println(decoder.readRaw());
  delay(500);
}
