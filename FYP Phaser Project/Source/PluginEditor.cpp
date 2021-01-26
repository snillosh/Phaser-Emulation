/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FYPPhaserProjectAudioProcessorEditor::FYPPhaserProjectAudioProcessorEditor (FYPPhaserProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    rateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    rateSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(rateSlider);
    
    depthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    depthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(depthSlider);
    
    feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(feedbackSlider);
    
    rateSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATE", rateSlider);
    
    depthSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DEPTH", depthSlider);
    
    feedbackSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackSlider);
}

FYPPhaserProjectAudioProcessorEditor::~FYPPhaserProjectAudioProcessorEditor()
{
}

//==============================================================================
void FYPPhaserProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Colours::black);
}

void FYPPhaserProjectAudioProcessorEditor::resized()
{
    rateSlider.setBounds(0, 0, 200, 100);
    depthSlider.setBounds(200, 100, 200, 100);
    feedbackSlider.setBounds(0, 100, 200, 100);
}
