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
    phaseIncrement = oscillatorFrequency / sampleRate;
    
    phasePosition = phasePosition + phaseIncrement;
    if (phasePosition > M_2_PI)
    {
        phasePosition = phasePosition - M_2_PI; //MathConstants<float>::twoPi;
        phasePosition = std::sin(phasePosition);
    }
    return phasePosition;
}

void SinOscillator::setFrequency(float frequency)
{
    oscillatorFrequency = frequency;
}

