/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>
#include "LFO.h"

//==============================================================================
/**
*/
class FYPPhaserProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FYPPhaserProjectAudioProcessor();
    ~FYPPhaserProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    void updateFilter();

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState apvts;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FYPPhaserProjectAudioProcessor)
    float lastSampleRate;
    OwnedArray<juce::dsp::FirstOrderTPTFilter<float>> filters;
    SinOscillator lfo;
    SinOscillator vibratoLFO;
    
    float rate = 1.0f, depth = 0.5f, feedback = 0.0f, mix = 0.5f, vibrato = 0.5f, feedbackGain = 0.5f;
    double lastOut = 0.0f;
    float centreFrequency = 1300.0f;
    static constexpr int numStages = 6;
    double sampleRate = 44100.0;
    float allPassOutFinal = 1.0f;
    
    AudioProcessorValueTreeState::ParameterLayout createParameters();
};
