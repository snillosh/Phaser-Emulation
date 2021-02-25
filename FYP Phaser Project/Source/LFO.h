/*
  ==============================================================================

    LFO.h
    Created: 26 Jan 2021 6:51:38pm
    Author:  Bevan Salter

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
/** A sine oscillator that oscillates between 1 and -1. Used for LFOs on modulation effects. Contains functions that set the sample rate and the amplitude of the wave (gain). The nextSample function returns the amplitude value for the next sample */
class SinOscillator
{
public:
    /** SinOscillator contructor*/
    SinOscillator();
    
    /** SinOscillator destructor*/
    ~SinOscillator();
    
    /** Sets the sample rate.
     @param newSampleRate new sample rate */
    void setSampleRate(float newSampleRate);
    
    /** Sets oscillator gain.
     @param newGain new gain value */
    void setGain(float newGain);
    
    /** Returns the next sample
     @return the next oscillator sample */
    float nextSample();
    
    /** Sets oscillator frequency
     @param frequency sets the oscillator frequency */
    void setFrequency (float frequency);
    
    
private:
    float sampleRate;
    float phasePosition = 0.f;
    float phaseIncrement = 0.f;
    float oscillatorFrequency = 0.5f;
    float gain = 1.0f;
};
