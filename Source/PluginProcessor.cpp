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
    //, valueTreeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    DBG("CONSTRUCTOR");

    for (int i = 0; i < 3; i++) {
        procGroup[i].setTreeStateReferences(&valueTreeState);
    }

    variableTree = {
        Parameters::variableTreeName, {},{
            {
                "Group", {{"name", "IR Vars"}},{
                    {"Parameter",{{"id", procOut.param_file}, {"value", "/"}}},
                    {"Parameter",{{"id", procOut.param_directory}, {"value", "/"}}},
                    {"Parameter",{{"id", procOut.param_stereoMode}, {"value", (int)Parameters::DUAL_MONO}}},
                    {"Parameter",{{"id", procLeft.param_file}, {"value", "/"}}},
                    {"Parameter",{{"id", procLeft.param_directory}, {"value", "/"}}},
                    {"Parameter",{{"id", procLeft.param_stereoMode}, {"value", (int)Parameters::DUAL_MONO}}},
                    {"Parameter",{{"id", procRight.param_file}, {"value", "/"}}},
                    {"Parameter",{{"id", procRight.param_directory}, {"value", "/"}}},
                    {"Parameter",{{"id", procRight.param_stereoMode}, {"value", (int)Parameters::DUAL_MONO}}}
                }
            }
        }
    };
}

//==============================================================================
void PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    DBG("prepareToPlay");

    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    for (int i = 0; i < 3; i++) {
        procGroup[i].setSpec(spec);
    }
}

void PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    //auto totalNumInputChannels  = getTotalNumInputChannels();
    //auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (int i = 0; i < 3; i++) {
        procGroup[i].updateParameters();
    }

    juce::dsp::AudioBlock<float> audioBlock = {buffer};
    juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    procLeft.process(context, buffer);
    procRight.process(context, buffer);
    juce::dsp::AudioBlock<float> audioBlockLeft = { procLeft.buffer };
    juce::dsp::AudioBlock<float> audioBlockRight = { procRight.buffer };
    juce::dsp::ProcessContextReplacing<float> contextLeft = juce::dsp::ProcessContextReplacing<float>(audioBlockLeft);
    juce::dsp::ProcessContextReplacing<float> contextRight = juce::dsp::ProcessContextReplacing<float>(audioBlockRight);
    
    procOut.process_combine(contextLeft, contextRight, procLeft.buffer, procRight.buffer);

    buffer.makeCopyOf(procOut.buffer);
    
}



//==============================================================================
void PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    DBG("getStateInformation - SAVE");

    valueTreeState.state.appendChild(variableTree, nullptr);
    
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    DBG("setStateInformation - LOAD");

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() == nullptr) {
        return;
    }
    if (!xmlState->hasTagName(valueTreeState.state.getType())) {
        return;
    }

    valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
    

    juce::ValueTree newValueTree = valueTreeState.state.getChildWithName(Parameters::variableTreeName);
    
    if (!newValueTree.isValid()) {
        return;
    }

    variableTree = valueTreeState.state.getChildWithName( Parameters::variableTreeName );
    if (!variableTree.isValid()) {
        return;
    }

    for (int i = 0; i < 3; i++) {
        procGroup[i].setStateInformation(&variableTree);
    }

    if (stateUpdate != nullptr) {
        stateUpdate();
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    for (int i = 0; i < 3; i++) {
        procGroup[i].setParameterLayout(params);
    }

    return { params.begin(), params.end() };
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
    DBG("_createEditor()");
    return new PluginAudioProcessorEditor(*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginAudioProcessor();
}

#pragma endregion
