/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class PluginAudioProcessor  : public juce::AudioProcessor
    , public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    PluginAudioProcessor();
    ~PluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState valueTreeState;
    juce::ValueTree variableTree;

    juce::File currentDirectory, currentFile;

    juce::dsp::Gain<float> inputGain;
    juce::dsp::Convolution convolution;
    juce::dsp::DryWetMixer<float> mixer;

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    std::function<void()> stateUpdate;
    void setFile(juce::File fileIr);

private:
    juce::AudioBuffer<float> mBufferConvolution;
    juce::dsp::ProcessSpec mSpec;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
