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
    freqSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    freqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(freqSlider);
    
    resSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    resSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(resSlider);
    
    gainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(gainSlider);
    
    freqSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FREQ", freqSlider);
    
    resSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RES", resSlider);
    
    gainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
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
    freqSlider.setBounds(0, 0, 200, 100);
    resSlider.setBounds(200, 100, 200, 100);
    gainSlider.setBounds(0, 100, 200, 100);
}
