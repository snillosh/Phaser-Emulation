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
    Slider rateSlider;
    Slider depthSlider;
    Slider feedbackSlider;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> rateSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> depthSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> feedbackSliderAttachment;
    
    FYPPhaserProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FYPPhaserProjectAudioProcessorEditor)
};
