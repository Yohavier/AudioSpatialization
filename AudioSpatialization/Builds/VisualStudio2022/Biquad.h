/*
  ==============================================================================

    biquad.h
    Created: 28 Jun 2022 10:16:29am
    Author:  Jonas

  ==============================================================================
*/

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

enum BiquadType
{
    None,
    Lowpass,
    Highpass,
    Peak,
    Bandpass,
    Notch,
    Lowshelf,
    Highshelf,
};

class Biquad
{
public:

    ~Biquad() = default;

    Biquad(const float sampleRate, const BiquadType bTypeParam, const float frequency, const float qualityFactor, const float peakGain)
        :m_sampleRate(sampleRate)
        , type(bTypeParam)
        , in_z1(0.0f)
        , in_z2(0.0f)
        , coeff_a0(0.0f)
        , coeff_a1(0.0f)
        , coeff_a2(0.0f)
        , coeff_b1(0.0f)
        , coeff_b2(0.0f)
    {
        ComputeCoeff(frequency, qualityFactor, peakGain);
    }

    // if bTypeParam is AkInt32 weird values come out
    void ComputeCoeff(const float frequency, const float qualityFactor, const float peakGain);
    float ProcessSample(float const sample);

private:
    float m_sampleRate;
    BiquadType type;

    float in_z1;
    float in_z2;

    float coeff_a0;
    float coeff_a1;
    float coeff_a2;
    float coeff_b1;
    float coeff_b2;
};