/*
  ==============================================================================

    LFO.h
    Created: 26 Jan 2021 6:51:38pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SinOscillator
{
public:
    SinOscillator();
    
    ~SinOscillator();
    
    void setSampleRate(float newSampleRate);
    
    void setGain(float newGain);
    
    float nextSample();
    
    void setFrequency (float frequency);
    
    
private:
    float sampleRate = 44100.f;
    float phasePosition = 0.f;
    float phaseIncrement = 0.f;
    float oscillatorFrequency = 0.5f;
    float gain = 1.0f;
};
