/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>

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
    void setRate(float newRate);
    void setDepth(float newDepth);
    void setCentreFrequency(float newCentreFrequecy);
    void setFeedback (float newFeedback);
    void setMix (float newMix);

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    AudioProcessorValueTreeState apvts;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FYPPhaserProjectAudioProcessor)
    float lastSampleRate;
    OwnedArray<juce::dsp::FirstOrderTPTFilter<float>> filters;
    juce::dsp::Oscillator<float> osc;
    SmoothedValue<float, ValueSmoothingTypes::Linear> oscVolume;
    
    float rate = 1.0f, depth = 0.5f, feedback = 0.0f, mix = 0.5f;
    float centreFrequency = 1300.0f;
    int numStages = 6;
    
    AudioProcessorValueTreeState::ParameterLayout createParameters();
};
