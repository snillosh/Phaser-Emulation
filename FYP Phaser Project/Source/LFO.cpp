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
    phaseIncrement = (M_2_PI * oscillatorFrequency) / sampleRate;
    
    phasePosition = phasePosition + phaseIncrement;
    if (phasePosition > M_2_PI)
    {
        phasePosition = phasePosition - M_2_PI;
    }
    return std::sin(phasePosition);
}

void SinOscillator::setFrequency(float frequency)
{
    oscillatorFrequency = frequency;
}

void SinOscillator::setSampleRate(float newSampleRate)
{
    sampleRate = newSampleRate;
}
