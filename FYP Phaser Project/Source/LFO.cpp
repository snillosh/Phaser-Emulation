/*
  ==============================================================================

    LFO.cpp
    Created: 26 Jan 2021 6:51:38pm
    Author:  Bevan Salter

  ==============================================================================
*/

#include "LFO.h"
#include <cmath>

SinOscillator::SinOscillator()
{
    
}

SinOscillator::~SinOscillator()
{
    
}


float SinOscillator::nextSample()
{
    phaseIncrement = (MathConstants<float>::twoPi * oscillatorFrequency) / sampleRate;
    
    phasePosition = phasePosition + phaseIncrement;
    if (phasePosition > MathConstants<float>::twoPi)
    {
        phasePosition = phasePosition - MathConstants<float>::twoPi;
    }
    return std::sin(phasePosition) * gain;
}

void SinOscillator::setFrequency(float frequency)
{
    oscillatorFrequency = frequency;
}

void SinOscillator::setSampleRate(float newSampleRate)
{
    sampleRate = newSampleRate;
}

void SinOscillator::setGain(float newGain)
{
    gain = newGain;
}
