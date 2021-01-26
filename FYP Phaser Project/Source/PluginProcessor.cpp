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
void FYPPhaserProjectAudioProcessor::setRate(float newRate)
{
    rate = newRate;
}

void FYPPhaserProjectAudioProcessor::setDepth(float newDepth)
{
    depth = newDepth;
}

void FYPPhaserProjectAudioProcessor::setCentreFrequency(float newCentreFrequecy)
{
    centreFrequency = newCentreFrequecy;
}

void FYPPhaserProjectAudioProcessor::setFeedback(float newFeedback)
{
    feedback = newFeedback;
}

void FYPPhaserProjectAudioProcessor::setMix(float newMix)
{
    mix = newMix;
}

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
    lastSampleRate = sampleRate;
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumInputChannels();
    spec.sampleRate = sampleRate;
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
    depth = *apvts.getRawParameterValue("DEPTH");
    lfo.setFrequency(rate);
    for (auto n = 0; n < numStages; ++n)
    {
        float phasePositionInHertz = (lfo.nextSample() * 20000.0f);
        filters[n]->setCutoffFrequency(phasePositionInHertz);
    }
}

void FYPPhaserProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    dsp::AudioBlock<float> block (buffer);
    //allPassFilter.process(dsp::ProcessContextReplacing<float>(block));
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            /*
            float filter1 = filters[0]->processSample(channel, channelData[sample]);
            float filter2 = filters[1]->processSample(channel, filter1);
            float filter3 = filters[2]->processSample(channel, filter2);
            float filter4 = filters[3]->processSample(channel, filter3);
            channelData [sample] = filter4;
             */
            updateFilter();
            float allPassOut1 = filters[0]->processSample(channel, channelData[sample]);
            float allPassOut2 = filters[1]->processSample(channel, allPassOut1);
            float allPassOut3 = filters[2]->processSample(channel, allPassOut2);
            float allPassOut4 = filters[3]->processSample(channel, allPassOut3);
            float allPassOut5 = filters[4]->processSample(channel, allPassOut4);
            float allPassOut6 = filters[5]->processSample(channel, allPassOut5);
                
            channelData[sample] = (depth * allPassOut6) + ((1.0f - depth) * channelData[sample]);
        }
    }
    
    
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
    params.push_back(std::make_unique<AudioParameterFloat>("RATE", "Rate", 0.02f, 10.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("DEPTH", "Depth", 0.1f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("GAIN", "Gain", 0.1f, 1.0f, 0.5f));
    
    return {params.begin(), params.end() };
}
