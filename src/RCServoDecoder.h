
#ifndef _RC_SERVO_DECODER_H_
#define _RC_SERVO_DECODER_H_

#include <Arduino.h>

/** Class to read RC servo pulse timings from a digital pin and map its
 * values into a certain range.
 */
class RCServoDecoder
{
public:
  /** Type used for time data within RCServoDecoder. */
  typedef uint16_t TimeDelta;
  /** Type used for mapped values within RCServoDecoder. */
  typedef int16_t Value;
  
  /** Constructor.
   * 
   * @param pinNr Number of the digital pin to which the RC receiver channel is
   *    attached to.
   * @param tmin The pulse length in microseconds which corresponds to the
   *    lowest setting on this channel
   * @param tmax The pulse length in microseconds which corresponds to the
   *    highest pulse length on this channel
   * @param omin The value which should be returned by read() when the lowest
   *    setting is present on the channel
   * @param omax The value which should be returned by read() when the highest
   *    setting is present on the channel
   * 
   * @note You can also invert the value mapping for instance
   *   omin =  100
   *   omax = -100
   */
  RCServoDecoder(byte pinNr,
                 TimeDelta tmin = 540,
                 TimeDelta tmax = 2400,
                 int16_t omin = -100,
                 int16_t omax = 100)
    : m_pinNr(pinNr)
    , m_tmin(tmin)
    , m_tmax(tmax)
    , m_omin(omin)
    , m_omax(omax)
  {}

  /** Initialize the servo decoder */
  void begin() const
  {
    pinMode(m_pinNr, INPUT);
  }

  /** Read the raw pulse length in microseconds - used within the calibration
   * tool
   */
  Value readRaw() const
  {
    return pulseIn(m_pinNr, HIGH);
  }

  /** Read the the RC channel and return the mapped value according to the
   * mapping passed in the constructor.
   */
  Value read() const
  {
    auto d = readRaw();

    /* map to desired ranges based input and output ranges */
    d = map(d, m_tmin, m_tmax, m_omin, m_omax);
    
    /* clip to output min/max -- handling inverse as well */
    if (m_omax > m_omin) {
      d = d > m_omax ? m_omax :
          d < m_omin ? m_omin :
          d;
    } else {
      d = d < m_omax ? m_omax :
          d > m_omin ? m_omin :
          d;
    }
    return d;
  }
  
private:
  byte m_pinNr;
  TimeDelta m_tmin;
  TimeDelta m_tmax;
  int16_t m_omin;
  int16_t m_omax;
};


#endif  /* _RC_SERVO_DECODER_H_ */

