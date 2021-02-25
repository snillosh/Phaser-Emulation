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
        filters[n]->snapToZero();
    }
    highpassFilter.add(new juce::dsp::FirstOrderTPTFilter<float>());
    highpassFilter[0]->setType(juce::dsp::FirstOrderTPTFilterType::highpass);
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
    highpassFilter[0]->prepare(spec);
    highpassFilter[0]->reset();
    highpassFilter[0]->setCutoffFrequency(50);
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
    input = (*apvts.getRawParameterValue("INPUT"));
    output = *apvts.getRawParameterValue("OUTPUT");
    output = output * output * output;
    
    lfo.setFrequency(rate);
    lfo.setGain(depth);
    vibratoLFO.setFrequency(vibrato);
    for (auto n = 0; n < numStages; ++n)
    {
        float phasePositionInHertz = (lfo.nextSample() * 0.5f) + 0.5f;
        phasePositionInHertz = (phasePositionInHertz * 4980.0f) + 20.0f;
        filters[n]->setCutoffFrequency(phasePositionInHertz);
        filters[n]->snapToZero();
    }
    feedbackGain = (vibratoLFO.nextSample() * 0.5f) + 0.5f;
}

float FYPPhaserProjectAudioProcessor::saturationTransfereFunction(float x)
{
    float y = 0.0f;
    float coeffA = 2.0f;
    if (x > 0.0f && x <= 1.0f)
    {
        y = (coeffA/(coeffA-1.0f))*(1.0f - pow(coeffA, -x));
    }
    else if (x <= 0.0f && x >= -1.0f)
    {
        y = (coeffA/(coeffA -1.0f))*(-1.0f + pow(coeffA, x));
    }
    return y;
}

void FYPPhaserProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
        auto* channelData = buffer.getWritePointer(0);
        
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        updateFilter();
        feedbackSaturationValue = (allPassOutFinal * feedback);
        
        if (feedbackSaturationValue > 1)
            feedbackSaturationValue = 1;
        else if (feedbackSaturationValue < -1)
            feedbackSaturationValue = -1;
        
        feedbackSaturationValue = (saturationTransfereFunction(feedbackSaturationValue) * input) + feedbackSaturationValue;
        
        //feedbackSaturationValue = highpassFilter[0]->processSample(0, feedbackSaturationValue);
        
        float allPassOut1 = filters[0]->processSample(0, channelData[sample]) + feedbackSaturationValue + (random.nextFloat() * 0.00025f - 0.000125f);
        float allPassOut2 = filters[1]->processSample(0, allPassOut1);
        float allPassOut3 = filters[2]->processSample(0, allPassOut2);
        allPassOutFinal = filters[3]->processSample(0, allPassOut3);
                
        channelData[sample] = (mix * allPassOutFinal) + ((1.0f - mix) * channelData[sample]);
        lastOut = channelData[sample];
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
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
    params.push_back(std::make_unique<AudioParameterFloat>("RATE", "Rate", 0.002f, 0.500f, 0.03f));
    params.push_back(std::make_unique<AudioParameterFloat>("MIX", "Mix", 0.1f, 1.0f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 0.99f, 0.8f));
    params.push_back(std::make_unique<AudioParameterFloat>("VIBRATO", "Vibrato", 0.001f, 1.000f, 0.00f ));
    params.push_back(std::make_unique<AudioParameterFloat>("DEPTH" , "Depth", 0.1f, 1.0f, 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("INPUT", "Input", 0.0f, .5f, 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("OUTPUT", "Ouput", 0.0f, 1.0f, 0.01f));
    
    
    return {params.begin(), params.end() };
}
