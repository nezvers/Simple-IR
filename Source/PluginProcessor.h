/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Globals.h"
#include "Library/LowHighCutFilters.h"

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

    void parameterChanged(const juce::String& parameterID, float newValue) override;
    
    void setIR1(juce::File fileIr);
    void setIR2(juce::File fileIr);
    // Sets stereo mode - mono, stereo, dual_mono
    void setStereo(Parameters::enumStereo value);
    std::function<void()> stateUpdate;


    juce::AudioProcessorValueTreeState valueTreeState;
    juce::ValueTree variableTree;

    juce::File currentDirectory1, currentFile1;
    juce::File currentDirectory2, currentFile2;
    
    bool invertedIR1 = false;
    bool invertedIR2 = false;
    bool bypassOut = false;
    bool bypassIR1 = false;
    bool bypassIR2 = false;

private:
    juce::dsp::DryWetMixer<float> mixer;
    juce::dsp::DryWetMixer<float> panIR1;
    juce::dsp::DryWetMixer<float> panIR2;
    juce::dsp::Gain<float> gainOut;
    juce::dsp::Gain<float> gainIR1;
    juce::dsp::Gain<float> gainIR2;
    juce::dsp::Convolution convolutionIR1;
    juce::dsp::Convolution convolutionIR2;
    LowHighCutFilters lowCutOut;
    LowHighCutFilters lowCutIR1;
    LowHighCutFilters lowCutIR2;
    LowHighCutFilters highCutOut;
    LowHighCutFilters highCutIR1;
    LowHighCutFilters highCutIR2;


    juce::AudioBuffer<float> mBufferIR1;
    juce::AudioBuffer<float> mBufferIR2;
    juce::dsp::ProcessSpec mSpec;
    Parameters::enumStereo mStereoMode;

    std::atomic<float>* mixValue;
    std::atomic<float>* gainValueOut;
    std::atomic<float>* gainValueIR1;
    std::atomic<float>* gainValueIR2;
    std::atomic<float>* bypassValueOut;
    std::atomic<float>* bypassValueIR1;
    std::atomic<float>* bypassValueIR2;
    std::atomic<float>* invertValueIR1;
    std::atomic<float>* invertValueIR2;
    std::atomic<float>* lowCutValueOut;
    std::atomic<float>* lowCutValueIR1;
    std::atomic<float>* lowCutValueIR2;
    std::atomic<float>* highCutValueOut;
    std::atomic<float>* highCutValueIR1;
    std::atomic<float>* highCutValueIR2;
    std::atomic<float>* panValueIR1;
    std::atomic<float>* panValueIR2;
    std::atomic<float>* delayValueIR1;
    std::atomic<float>* delayValueIR2;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void linkParameters();
    void updateParameters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
