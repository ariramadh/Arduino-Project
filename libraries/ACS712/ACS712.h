#pragma once
//
//    FILE: ACS712.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.3
//    DATE: 2020-03-17
// PURPOSE: ACS712 library - current measurement
//
// Tested with a RobotDyn ACS712 20A breakout + UNO.
//

#include "Arduino.h"

#define ACS712_LIB_VERSION "0.1.3"


class ACS712
{
  public:
    //  NOTE:
    //  One can quite precisely tune the value of the sensor
    //      (1) the milliVolt per Ampere and
    //      (2) the volts parameter.
    //
    //  TYPE  mV per Ampere
    //  5A    185
    //  20A   100
    //  30A   66
    ACS712(uint8_t analogPin, float volts = 5.0, uint16_t maxADC = 1023, uint8_t mVperA = 100);

    // returns mA
    // blocks 20-21 ms to sample a whole 50 or 60 Hz period.
    int        mA_AC(uint8_t freq = 50);

    // returns mA
    // blocks < 1 ms
    int        mA_DC();

    // midpoint ADC for DC only
    inline void setMidPoint(uint16_t mp) { _midPoint = mp; };
    inline uint16_t getMidPoint() { return _midPoint; };
    inline void     incMidPoint() { _midPoint++; };
    inline void     decMidPoint() { _midPoint--; };

    // also known as crest factor;  affects AC only
    inline void     setFormFactor(float ff) { _formFactor = ff; };
    inline float    getFormFactor() { return _formFactor; };

    // AC and DC
    inline void     setmVperAmp(uint8_t mva) { _mVperAmpere = mva; };
    inline uint8_t  getmVperAmp() { return _mVperAmpere; };

  private:
    uint8_t   _pin;
    float     _mVpstep;     // millivolt per step
    float     _formFactor;    // P2P -> RMS
    uint8_t   _mVperAmpere;
    uint16_t  _midPoint;
};

// END OF FILE
