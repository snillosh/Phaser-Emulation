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
    setSize (800, 600);
    rateSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    rateSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(rateSlider);
    rateLabel.setText("Rate", dontSendNotification);
    rateLabel.attachToComponent(&rateSlider, true);
    addAndMakeVisible(rateLabel);
    
    depthSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    depthSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(depthSlider);
    depthLabel.setText("Depth", dontSendNotification);
    depthLabel.attachToComponent(&depthSlider, true);
    addAndMakeVisible(depthLabel);
    
    feedbackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    feedbackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(feedbackSlider);
    feedbackLabel.setText("Feedback", dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, true);
    addAndMakeVisible(feedbackLabel);
    
    vibratoSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    vibratoSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(vibratoSlider);
    vibratoLabel.setText("Vibrato", dontSendNotification);
    vibratoLabel.attachToComponent(&vibratoSlider, true);
    addAndMakeVisible(vibratoLabel);
    
    mixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(mixSlider);
    mixLabel.setText("Mix", dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, true);
    addAndMakeVisible(mixLabel);
    
    inGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    inGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(inGainSlider);
    inLabel.setText("Input Gain", dontSendNotification);
    inLabel.attachToComponent(&inGainSlider, true);
    addAndMakeVisible(inLabel);
    
    outGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    outGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 50);
    addAndMakeVisible(outGainSlider);
    outLabel.setText("Output Gain", dontSendNotification);
    outLabel.attachToComponent(&outGainSlider, true);
    addAndMakeVisible(outLabel);
    
    
    rateSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATE", rateSlider);
    
    depthSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DEPTH", depthSlider);
    
    feedbackSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedbackSlider);
    
    vibratoSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "VIBRATO", vibratoSlider);
    
    mixSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "MIX", mixSlider);
    
    inGainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "INPUT", inGainSlider);
    
    outGainSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "OUTPUT", outGainSlider);
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
    rateSlider.setBounds(200, 0, 200, 100);
    depthSlider.setBounds(500, 100, 200, 100);
    feedbackSlider.setBounds(200, 100, 200, 100);
    vibratoSlider.setBounds(500, 0 , 200, 100);
    mixSlider.setBounds(200, 200, 200, 100);
    inGainSlider.setBounds(200, 300, 200, 100);
    outGainSlider.setBounds(500, 300, 200, 100);
}
