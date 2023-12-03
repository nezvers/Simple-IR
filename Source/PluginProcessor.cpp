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
    linkParameters();
    currentFile1 = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSpec.maximumBlockSize = samplesPerBlock;
    mSpec.sampleRate = sampleRate;
    mSpec.numChannels = getTotalNumOutputChannels();

    gainOut.prepare(mSpec);
    gainIR1.prepare(mSpec);
    gainIR2.prepare(mSpec);

    mixer.prepare(mSpec);
    mixer.setWetMixProportion(0.0f);
    mixer.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    panIR1.prepare(mSpec);
    panIR1.setWetMixProportion(0.5f);
    panIR1.setMixingRule(juce::dsp::DryWetMixingRule::squareRoot3dB);
    panIR2.prepare(mSpec);
    panIR2.setWetMixProportion(0.5f);
    panIR2.setMixingRule(juce::dsp::DryWetMixingRule::squareRoot3dB);
    
    convolutionIR1.prepare(mSpec);
    convolutionIR2.prepare(mSpec);

    lowCutOut.sampleRate = sampleRate;
    lowCutIR1.sampleRate = sampleRate;
    lowCutIR2.sampleRate = sampleRate;
    highCutOut.sampleRate = sampleRate;
    highCutIR1.sampleRate = sampleRate;
    highCutIR2.sampleRate = sampleRate;
    lowCutOut.type = LowHighCutFilters::LOWCUT;
    lowCutIR1.type = LowHighCutFilters::LOWCUT;
    lowCutIR2.type = LowHighCutFilters::LOWCUT;
    highCutOut.type = LowHighCutFilters::HIGHCUT;
    highCutIR1.type = LowHighCutFilters::HIGHCUT;
    highCutIR2.type = LowHighCutFilters::HIGHCUT;

    DBG("PREPARE TO PLAY");
    
    double valueGain = valueTreeState.getRawParameterValue(Parameters::id_gainOut)->load();

    gainOut.setRampDurationSeconds(0.01);
    gainOut.setGainDecibels(valueGain);
    // TODO: initialize after state set.
}

void PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();

    mBufferIR1.makeCopyOf(buffer, true);
    mBufferIR2.makeCopyOf(buffer, true);

    juce::dsp::AudioBlock<float> audioBlockIR1 {mBufferIR1};
    juce::dsp::AudioBlock<float> audioBlockIR2 {mBufferIR2};
    
    
    juce::dsp::ProcessContextReplacing contextConvolutionIR1 = juce::dsp::ProcessContextReplacing<float>(audioBlockIR1);
    juce::dsp::ProcessContextReplacing contextConvolutionIR2 = juce::dsp::ProcessContextReplacing<float>(audioBlockIR2);

    if (convolutionIR1.getCurrentIRSize() > 0) {
        convolutionIR1.process(contextConvolutionIR1);
    }
    if (convolutionIR2.getCurrentIRSize() > 0) {
        convolutionIR2.process(contextConvolutionIR2);
    }

    mixer.pushDrySamples(contextConvolutionIR1.getOutputBlock());
    mixer.mixWetSamples(contextConvolutionIR2.getOutputBlock()); // mix into IR2 buffer

    gainOut.process(contextConvolutionIR2);

    buffer.makeCopyOf(mBufferIR1); // buffer from IR2
    
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
    mStereoMode = (Parameters::enumStereo) int( variableTree.getProperty( Parameters::param_stereoMode ) );
    
    if (!currentFile1.existsAsFile()) {
        return;
    }

    convolutionIR1.loadImpulseResponse(currentFile1,
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
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_mix, Parameters::name_mix, Parameters::panMin, Parameters::panMax, 0.0f) );
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_gainOut, Parameters::name_gainOut, Parameters::gainMin, Parameters::gainMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_gainIR1, Parameters::name_gainIR1, Parameters::gainMin, Parameters::gainMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_gainIR2, Parameters::name_gainIR2, Parameters::gainMin, Parameters::gainMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_lowCutOut, Parameters::name_lowCutOut, Parameters::freqMin, Parameters::freqMax, Parameters::freqMin));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_lowCutIR1, Parameters::name_lowCutIR1, Parameters::freqMin, Parameters::freqMax, Parameters::freqMin));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_lowCutIR2, Parameters::name_lowCutIR2, Parameters::freqMin, Parameters::freqMax, Parameters::freqMin));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_hiCutOut, Parameters::name_hiCutOut, Parameters::freqMin, Parameters::freqMax, Parameters::freqMax));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_hiCutIR1, Parameters::name_hiCutIR1, Parameters::freqMin, Parameters::freqMax, Parameters::freqMax));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_hiCutIR2, Parameters::name_hiCutIR2, Parameters::freqMin, Parameters::freqMax, Parameters::freqMax));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_panIR1, Parameters::name_panIR1, Parameters::panMin, Parameters::panMax, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_panIR2, Parameters::name_panIR2, Parameters::panMin, Parameters::panMax, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_delayIR1, Parameters::name_delayIR1, Parameters::panMin, Parameters::panMax, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_delayIR2, Parameters::name_delayIR2, Parameters::panMin, Parameters::panMax, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_bypassOut, Parameters::name_bypassOut, Parameters::panMin, Parameters::panMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_bypassIR1, Parameters::name_bypassIR1, Parameters::panMin, Parameters::panMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_bypassIR2, Parameters::name_bypassIR2, Parameters::panMin, Parameters::panMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_invertIR1, Parameters::name_invertIR1, Parameters::panMin, Parameters::panMax, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(Parameters::id_invertIR2, Parameters::name_invertIR2, Parameters::panMin, Parameters::panMax, 0.0f));

    variableTree = {
        Parameters::variableTreeName, {},{
            {
                "Group", {{"name", "IR Vars"}},{
                    {"Parameter",{{"id", Parameters::param_file1}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file2}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file1Directory}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_file2Directory}, {"value", "/"}}},
                    {"Parameter",{{"id", Parameters::param_stereoMode}, {"value", Parameters::DUAL_MONO}}},
                }
            }
        }
    };

    return { params.begin(), params.end() };
}

void PluginAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // TODO: optimize checking
    if (parameterID == Parameters::id_gainOut) {
        DBG("GAIN CHANGED");
        double valueGain = valueTreeState.getRawParameterValue(Parameters::id_gainOut)->load();
        gainOut.setGainDecibels(valueGain);
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
    convolutionIR1.loadImpulseResponse(currentFile1,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
    if (stateUpdate != nullptr && currentFile1.existsAsFile()) {
        stateUpdate();
    }
}

void PluginAudioProcessor::setIR2(juce::File fileIr)
{
    currentFile2 = fileIr;
    currentDirectory2 = fileIr.getParentDirectory().getFullPathName();

    variableTree.setProperty(Parameters::param_file2, fileIr.getFullPathName(), nullptr);
    variableTree.setProperty(Parameters::param_file2Directory, fileIr.getParentDirectory().getFullPathName(), nullptr);
    convolutionIR2.loadImpulseResponse(currentFile1,
        juce::dsp::Convolution::Stereo::yes,
        juce::dsp::Convolution::Trim::yes,
        0);
    if (stateUpdate != nullptr && currentFile1.existsAsFile()) {
        stateUpdate();
    }
}

void PluginAudioProcessor::linkParameters()
{
    mixValue = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    gainValueOut = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    gainValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    gainValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    bypassValueOut = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    bypassValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    bypassValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    invertValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    invertValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    lowCutValueOut = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    lowCutValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    lowCutValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    highCutValueOut = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    highCutValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    highCutValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    panValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    panValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    delayValueIR1 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
    delayValueIR2 = valueTreeState.getRawParameterValue(Parameters::id_gainOut);
}

void PluginAudioProcessor::updateParameters()
{
    mixer.setWetMixProportion(mixValue->load());
    gainOut.setGainDecibels(gainValueOut->load());
    gainIR1.setGainDecibels(gainValueIR1->load());
    gainIR2.setGainDecibels(gainValueIR2->load());
}

void PluginAudioProcessor::setStereo(Parameters::enumStereo value)
{
    variableTree.setProperty(Parameters::param_stereoMode, value, nullptr);
}



#pragma region DEFAULT

PluginAudioProcessor::~PluginAudioProcessor()
{

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
