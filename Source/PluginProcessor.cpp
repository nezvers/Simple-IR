/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters\Globals.h"

//==============================================================================
PluginAudioProcessor::PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    , valueTreeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    valueTreeState.addParameterListener(Parameters::inputGainId, this);
    valueTreeState.addParameterListener(Parameters::mixId, this);

    variableTree = {
        Parameters::variableTreeName, {},{
            {
                "Group", {{"name", "IR Vars"}},{
                    {"Parameter",{{"id", Parameters::file1}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::root}, {"value", "/"}}}
                }
            }
        }
    };
}

PluginAudioProcessor::~PluginAudioProcessor()
{
    valueTreeState.removeParameterListener(Parameters::inputGainId, this);
    valueTreeState.removeParameterListener(Parameters::mixId, this);
}

//==============================================================================
const juce::String PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void PluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSpec.maximumBlockSize = samplesPerBlock;
    mSpec.sampleRate = sampleRate;
    mSpec.numChannels = getTotalNumOutputChannels();
    
    mixer.setMixingRule(juce::dsp::DryWetMixingRule::squareRoot3dB);
    //mBufferConvolution.setSize(2, samplesPerBlock, true, true, true);

    inputGain.prepare(mSpec);
    convolution.prepare(mSpec);
    mixer.prepare(mSpec);
    mixer.setWetMixProportion(0.0f);

    DBG("PREPARE TO PLAY");
    


    double valueGain = valueTreeState.getRawParameterValue(Parameters::inputGainId)->load();

    inputGain.setRampDurationSeconds(0.01);
    inputGain.setGainDecibels(valueGain);
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    mBufferConvolution.makeCopyOf(buffer, true);

    juce::dsp::AudioBlock<float> audioBlockInput {buffer};
    juce::dsp::AudioBlock<float> audioBlockConvolver {mBufferConvolution};
    
    juce::dsp::ProcessContextReplacing contextInput = juce::dsp::ProcessContextReplacing<float>(audioBlockInput);
    juce::dsp::ProcessContextReplacing contextConvolution = juce::dsp::ProcessContextReplacing<float>(audioBlockConvolver);

    if (convolution.getCurrentIRSize() > 0) {
        convolution.process(contextConvolution);
    }

    auto convolutionOutput = contextConvolution.getOutputBlock();
    mixer.pushDrySamples(audioBlockInput);
    mixer.mixWetSamples(convolutionOutput);

    inputGain.process(contextConvolution);
    buffer.makeCopyOf(mBufferConvolution);
    /*if (convolution.getCurrentIRSize() > 0) {
        convolution.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    }
    inputGain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));*/

    /*
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
    */
}


//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor (*this);
}

//==============================================================================
void PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    valueTreeState.state.appendChild(variableTree, nullptr);
    
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() == nullptr) {
        return;
    }
    if (!xmlState->hasTagName(valueTreeState.state.getType())) {
        return;
    }

    valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    juce::ValueTree newValueTree = valueTreeState.state.getChildWithName(Parameters::variableTreeName);
    DBG("SET STATE");
    if (!newValueTree.isValid()) {
        DBG("NOT VALID VARIABLE TREE");
        return;
    }
    variableTree = valueTreeState.state.getChildWithName(Parameters::variableTreeName);
    currentFile = juce::File(variableTree.getProperty(Parameters::file1));
    currentDirectory = juce::File(variableTree.getProperty(Parameters::root));
    if (!currentFile.existsAsFile()) {
        return;
    }

    convolution.loadImpulseResponse(currentFile,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);

    if (stateUpdate != nullptr) {
        stateUpdate();
    }

}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    //params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::parameterId, Parameters::parameterName, -24.0f, 24.0f, 0.0f));
    params.push_back( std::make_unique<juce::AudioParameterFloat>(Parameters::mixId, Parameters::mixName, Parameters::panMin, Parameters::panMax, 0.5f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat>(Parameters::inputGainId, Parameters::inputGainName, Parameters::gainMin, Parameters::gainMax, 0.0f) );

    return { params.begin(), params.end() };
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == Parameters::inputGainId) {
        double valueGain = valueTreeState.getRawParameterValue(Parameters::inputGainId)->load();
        inputGain.setGainDecibels(valueGain);
    }
    if (parameterID == Parameters::mixId) {
        double convolutionMix = valueTreeState.getRawParameterValue(Parameters::mixId)->load();
        mixer.setWetMixProportion(convolutionMix);
    }
}

void PluginAudioProcessor::setFile(juce::File fileIr)
{
    currentFile = fileIr;
    currentDirectory = fileIr.getParentDirectory().getFullPathName();
    DBG(fileIr.getParentDirectory().getFullPathName());
    variableTree.setProperty(Parameters::file1, fileIr.getFullPathName(), nullptr);
    variableTree.setProperty(Parameters::root, fileIr.getParentDirectory().getFullPathName(), nullptr);
    convolution.loadImpulseResponse(currentFile,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
    if (stateUpdate != nullptr && currentFile.existsAsFile()) {
        stateUpdate();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}
