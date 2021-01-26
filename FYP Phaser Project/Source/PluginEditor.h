/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FYPPhaserProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FYPPhaserProjectAudioProcessorEditor (FYPPhaserProjectAudioProcessor&);
    ~FYPPhaserProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Slider freqSlider;
    Slider resSlider;
    Slider gainSlider;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> resSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainSliderAttachment;
    
    FYPPhaserProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FYPPhaserProjectAudioProcessorEditor)
};
