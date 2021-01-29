/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FYPPhaserProjectAudioProcessor::FYPPhaserProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    for (auto n = 0; n < numStages; ++n)
    {
        filters.add (new juce::dsp::FirstOrderTPTFilter<float>());
        filters[n]->setType (juce::dsp::FirstOrderTPTFilterType::allpass);
    }
}

FYPPhaserProjectAudioProcessor::~FYPPhaserProjectAudioProcessor()
{
}

//==============================================================================

const juce::String FYPPhaserProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FYPPhaserProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FYPPhaserProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FYPPhaserProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FYPPhaserProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FYPPhaserProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FYPPhaserProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FYPPhaserProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FYPPhaserProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void FYPPhaserProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FYPPhaserProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastOut = 0.0f;
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    spec.sampleRate = sampleRate;
    
    lfo.setSampleRate(sampleRate);
    
    vibratoLFO.setSampleRate(sampleRate);
    
    for (auto n = 0; n < numStages; ++n)
    {
        filters[n]->prepare (spec);
        filters[n]->reset();
    }
}

void FYPPhaserProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FYPPhaserProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FYPPhaserProjectAudioProcessor::updateFilter()
{
    rate = *apvts.getRawParameterValue("RATE");
    mix = *apvts.getRawParameterValue("MIX");
    feedback = *apvts.getRawParameterValue("FEEDBACK");
    vibrato = *apvts.getRawParameterValue("VIBRATO");
    depth = *apvts.getRawParameterValue("DEPTH");
    lfo.setFrequency(rate);
    lfo.setGain(depth);
    vibratoLFO.setFrequency(vibrato);
    for (auto n = 0; n < numStages; ++n)
    {
        float phasePositionInHertz = ((lfo.nextSample() * 12650.0f) + 550.0f);
        if (phasePositionInHertz < 0)
            phasePositionInHertz = phasePositionInHertz * -1.0f;
        filters[n]->setCutoffFrequency(phasePositionInHertz);
    }
    feedbackGain = vibratoLFO.nextSample();
    if (feedbackGain < 0)
        feedbackGain = feedbackGain * -1.0f;
}

void FYPPhaserProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            updateFilter();
            float allPassOut1 = filters[0]->processSample(channel, channelData[sample]) + (allPassOutFinal * feedback * feedbackGain);
            float allPassOut2 = filters[1]->processSample(channel, allPassOut1);
            float allPassOut3 = filters[2]->processSample(channel, allPassOut2);
            float allPassOut4 = filters[3]->processSample(channel, allPassOut3);
            float allPassOut5 = filters[4]->processSample(channel, allPassOut4);
            allPassOutFinal = filters[5]->processSample(channel, allPassOut5);
                
            channelData[sample] = (mix * allPassOutFinal) + ((1.0f - mix) * channelData[sample]);
            lastOut = channelData[sample];
        }
    }
    /*
    if (totalNumInputChannels == 2)
    {
        buffer.addFrom(0, 0, buffer, 1, 0, buffer.getNumSamples());
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
        buffer.applyGain(0.5f);
    }
     */
}

//==============================================================================
bool FYPPhaserProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FYPPhaserProjectAudioProcessor::createEditor()
{
    return new FYPPhaserProjectAudioProcessorEditor (*this);
}

//==============================================================================
void FYPPhaserProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FYPPhaserProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FYPPhaserProjectAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout FYPPhaserProjectAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    params.push_back(std::make_unique<AudioParameterFloat>("RATE", "Rate", 0.002f, 1.000f, 0.020f));
    params.push_back(std::make_unique<AudioParameterFloat>("MIX", "Mix", 0.1f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("FEEDBACK", "Feedback", 0.1f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("VIBRATO", "Vibrato", 0.001f, 1.000f, 0.02f ));
    params.push_back(std::make_unique<AudioParameterFloat>("DEPTH" , "Depth", 0.1f, 1.0f, 0.5f));
    
    return {params.begin(), params.end() };
}
