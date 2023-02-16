//
//  FixedPointBiquad.h
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//

#ifndef FixedPointBiquad_h
#define FixedPointBiquad_h

#include <Arduino.h>

enum {
    bq_type_lowpass = 0,
    bq_type_highpass,
    bq_type_bandpass,
    bq_type_notch,
    bq_type_peak,
    bq_type_lowshelf,
    bq_type_highshelf
};

class FixedPointBiquad {
public:
    FixedPointBiquad();
    FixedPointBiquad(int type, double Fc, double Q, double peakGainDB);
    ~FixedPointBiquad();
    void setType(int type);
    void setQ(double Q);
    void setFc(double Fc);
    void setPeakGain(double peakGainDB);
    void setBiquad(int type, double Fc, double Q, double peakGain);
    int16_t process(int16_t in);
    
protected:
    void calcBiquad(void);

    int type;
    int32_t a0, a1, a2, b1, b2;
    double Fc, Q, peakGain;
    int32_t z1, z2;
};

inline int16_t FixedPointBiquad::process(int16_t in) {
    int16_t out = in * a0 / 10000 + z1;
    z1 = in * a1 / 10000 + z2 - b1 * out / 10000;
    z2 = in * a2 / 10000 - b2 * out / 10000;
    return out;
}

#endif // FixedPointBiquad_h
