//
//  FixedPointBiquad.h
//
//  Marcin Kopa: float version
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

#include <math.h>
#include "FixedPointBiquad.h"

FixedPointBiquad::FixedPointBiquad()
{
    type = bq_type_lowpass;
    a0 = 1.0f;
    a1 = a2 = b1 = b2 = 0.0f;
    Fc = 0.50f;
    Q = 0.707f;
    peakGain = 0.0f;
    z1 = z2 = 0.0f;
}

FixedPointBiquad::FixedPointBiquad(int type, float Fc, float Q, float peakGainDB)
{
    setBiquad(type, Fc, Q, peakGainDB);
    z1 = z2 = 0.0f;
}

FixedPointBiquad::~FixedPointBiquad()
{
}

void FixedPointBiquad::setType(int type)
{
    this->type = type;
    calcBiquad();
}

void FixedPointBiquad::setQ(float Q)
{
    this->Q = Q;
    calcBiquad();
}

void FixedPointBiquad::setFc(float Fc)
{
    this->Fc = Fc;
    calcBiquad();
}

void FixedPointBiquad::setPeakGain(float peakGainDB)
{
    this->peakGain = peakGainDB;
    calcBiquad();
}

void FixedPointBiquad::setBiquad(int type, float Fc, float Q, float peakGainDB)
{
    this->type = type;
    this->Q = Q;
    this->Fc = Fc;
    setPeakGain(peakGainDB);
}

void FixedPointBiquad::calcBiquad(void)
{
    float norm;
    float V = powf(10, fabsf(peakGain) / 20.0f);
    float K = tanf(M_PI * Fc);
    switch (this->type)
    {
    case bq_type_lowpass:
        norm = 1.0f / (1.0f + K / Q + K * K);
        a0 = K * K * norm;
        a1 = 2.0f * a0;
        a2 = a0;
        b1 = 2.0f * (K * K - 1.0f) * norm;
        b2 = (1.0f - K / Q + K * K) * norm;
        break;

    case bq_type_highpass:
        norm = 1.0f / (1.0f + K / Q + K * K);
        a0 = 1.0f * norm;
        a1 = -2.0f * a0;
        a2 = a0;
        b1 = 2.0f * (K * K - 1.0f) * norm;
        b2 = (1.0f - K / Q + K * K) * norm;
        break;

    case bq_type_bandpass:
        norm = 1.0f / (1.0f + K / Q + K * K);
        a0 = K / Q * norm;
        a1 = 0.0f;
        a2 = -a0;
        b1 = 2.0f * (K * K - 1.0f) * norm;
        b2 = (1.0f - K / Q + K * K) * norm;
        break;

    case bq_type_notch:
        norm = 1.0f / (1.0f + K / Q + K * K);
        a0 = (1.0f + K * K) * norm;
        a1 = 2.0f * (K * K - 1.0f) * norm;
        a2 = a0;
        b1 = a1;
        b2 = (1.0f - K / Q + K * K) * norm;
        break;

    case bq_type_peak:
        if (peakGain >= 0)
        { // boost
            norm = 1.0f / (1.0f + 1.0f / Q * K + K * K);
            a0 = (1.0f + V / Q * K + K * K) * norm;
            a1 = 2.0f * (K * K - 1.0f) * norm;
            a2 = (1.0f - V / Q * K + K * K) * norm;
            b1 = a1;
            b2 = (1.0f - 1.0f / Q * K + K * K) * norm;
        }
        else
        { // cut
            norm = 1.0f / (1.0f + V / Q * K + K * K);
            a0 = (1.0f + 1.0f / Q * K + K * K) * norm;
            a1 = 2.0f * (K * K - 1.0f) * norm;
            a2 = (1.0f - 1.0f / Q * K + K * K) * norm;
            b1 = a1;
            b2 = (1.0f - V / Q * K + K * K) * norm;
        }
        break;
    case bq_type_lowshelf:
        if (peakGain >= 0)
        { // boost
            norm = 1.0f / (1.0f + sqrtf(2.0f) * K + K * K);
            a0 = (1.0f + sqrtf(2.0f * V) * K + V * K * K) * norm;
            a1 = 2.0f * (V * K * K - 1.0f) * norm;
            a2 = (1.0f - sqrtf(2.0f * V) * K + V * K * K) * norm;
            b1 = 2.0f * (K * K - 1.0f) * norm;
            b2 = (1.0f - sqrtf(2.0f) * K + K * K) * norm;
        }
        else
        { // cut
            norm = 1 / (1.0f + sqrtf(2.0f * V) * K + V * K * K);
            a0 = (1.0f + sqrtf(2.0f) * K + K * K) * norm;
            a1 = 2.0f * (K * K - 1.0f) * norm;
            a2 = (1.0f - sqrtf(2.0f) * K + K * K) * norm;
            b1 = 2.0f * (V * K * K - 1.0f) * norm;
            b2 = (1.0f - sqrtf(2.0f * V) * K + V * K * K) * norm;
        }
        break;
    case bq_type_highshelf:
        if (peakGain >= 0)
        { // boost
            norm = 1 / (1.0f + sqrtf(2.0f) * K + K * K);
            a0 = (V + sqrtf(2.0f * V) * K + K * K) * norm;
            a1 = 2.0f * (K * K - V) * norm;
            a2 = (V - sqrtf(2.0f * V) * K + K * K) * norm;
            b1 = 2.0f * (K * K - 1.0f) * norm;
            b2 = (1.0f - sqrtf(2.0f) * K + K * K) * norm;
        }
        else
        { // cut
            norm = 1.0f / (V + sqrtf(2.0f * V) * K + K * K);
            a0 = (1.0f + sqrtf(2.0f) * K + K * K) * norm;
            a1 = 2.0f * (K * K - 1.0f) * norm;
            a2 = (1.0f - sqrtf(2.0f) * K + K * K) * norm;
            b1 = 2.0f * (K * K - V) * norm;
            b2 = (V - sqrtf(2.0f * V) * K + K * K) * norm;
        }
        break;
    }

    return;
}
