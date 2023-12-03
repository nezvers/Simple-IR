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
    currentFile1 = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    valueTreeState.addParameterListener(Parameters::id_gainOut, this);
    valueTreeState.addParameterListener(Parameters::id_mix, this);

    variableTree = {
        Parameters::variableTreeName, {},{
            {
                "Group", {{"name", "IR Vars"}},{
                    {"Parameter",{{"id", Parameters::param_file1}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file2}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file1Directory}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file2Directory}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_stereo}, {"value", Parameters::DUAL_MONO}}},
                }
            }
        }
    };
}

PluginAudioProcessor::~PluginAudioProcessor()
{
    valueTreeState.removeParameterListener(Parameters::id_gainOut, this);
    valueTreeState.removeParameterListener(Parameters::id_mix, this);
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSpec.maximumBlockSize = samplesPerBlock;
    mSpec.sampleRate = sampleRate;
    mSpec.numChannels = getTotalNumOutputChannels();
    
    mixer.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    //mBufferConvolution.setSize(2, samplesPerBlock, true, true, true);

    outputGain.prepare(mSpec);
    convolution1.prepare(mSpec);
    convolution2.prepare(mSpec);
    mixer.prepare(mSpec);
    mixer.setWetMixProportion(0.0f);

    DBG("PREPARE TO PLAY");
    


    double valueGain = valueTreeState.getRawParameterValue(Parameters::id_gainOut)->load();

    outputGain.setRampDurationSeconds(0.01);
    outputGain.setGainDecibels(valueGain);
}

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

    if (convolution1.getCurrentIRSize() > 0) {
        convolution1.process(contextConvolution);
    }

    juce::dsp::AudioBlock convolutionOutput = contextConvolution.getOutputBlock();
    mixer.pushDrySamples(audioBlockInput);
    mixer.mixWetSamples(convolutionOutput);

    outputGain.process(contextConvolution);
    buffer.makeCopyOf(mBufferConvolution);
    
}



//==============================================================================
void PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    valueTreeState.state.appendChild(variableTree, nullptr);
    
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
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
        return;
    }

    variableTree = valueTreeState.state.getChildWithName( Parameters::variableTreeName );
    currentFile1 = juce::File( variableTree.getProperty( Parameters::param_file1 ) );
    currentFile2 = juce::File( variableTree.getProperty( Parameters::param_file2 ) );
    currentDirectory1 = juce::File( variableTree.getProperty( Parameters::param_file1Directory ) );
    currentDirectory2 = juce::File( variableTree.getProperty( Parameters::param_file2Directory ) );
    mStereoMode = (Parameters::enumStereo) int( variableTree.getProperty( Parameters::param_stereo ) );
    
    if (!currentFile1.existsAsFile()) {
        return;
    }

    convolution1.loadImpulseResponse(currentFile1,
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
    params.push_back( std::make_unique<juce::AudioParameterFloat>(Parameters::id_mix, Parameters::name_mix, Parameters::panMin, Parameters::panMax, 0.5f) );
    params.push_back( std::make_unique<juce::AudioParameterFloat>(Parameters::id_gainOut, Parameters::name_gainOut, Parameters::gainMin, Parameters::gainMax, 0.0f));
    params.push_back( std::make_unique<juce::AudioParameterBool>(Parameters::id_bypass, Parameters::name_bypass, true, Parameters::buttonAtributes) );

    return { params.begin(), params.end() };
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == Parameters::id_gainOut) {
        double valueGain = valueTreeState.getRawParameterValue(Parameters::id_gainOut)->load();
        outputGain.setGainDecibels(valueGain);
    }
    if (parameterID == Parameters::id_mix) {
        double convolutionMix = valueTreeState.getRawParameterValue(Parameters::id_mix)->load();
        mixer.setWetMixProportion(convolutionMix);
    }
}

void PluginAudioProcessor::setIR1(juce::File fileIr)
{
    currentFile1 = fileIr;
    currentDirectory1 = fileIr.getParentDirectory().getFullPathName();

    variableTree.setProperty(Parameters::param_file1, fileIr.getFullPathName(), nullptr);
    variableTree.setProperty(Parameters::param_file1Directory, fileIr.getParentDirectory().getFullPathName(), nullptr);
    convolution1.loadImpulseResponse(currentFile1,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
    if (stateUpdate != nullptr && currentFile1.existsAsFile()) {
        stateUpdate();
    }
}

void PluginAudioProcessor::setIR2(juce::File fileIr)
{
    currentFile1 = fileIr;
    currentDirectory1 = fileIr.getParentDirectory().getFullPathName();

    variableTree.setProperty(Parameters::param_file2, fileIr.getFullPathName(), nullptr);
    variableTree.setProperty(Parameters::param_file2Directory, fileIr.getParentDirectory().getFullPathName(), nullptr);
    convolution1.loadImpulseResponse(currentFile1,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
    if (stateUpdate != nullptr && currentFile1.existsAsFile()) {
        stateUpdate();
    }
}

void PluginAudioProcessor::setStereo(Parameters::enumStereo value)
{
    variableTree.setProperty(Parameters::param_stereo, value, nullptr);
}



#pragma region DEFAULT

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

void PluginAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String PluginAudioProcessor::getProgramName(int index)
{
    return {};
}

void PluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

void PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
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

//==============================================================================
bool PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginAudioProcessor::createEditor()
{
    return new PluginAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}

#pragma endregion
