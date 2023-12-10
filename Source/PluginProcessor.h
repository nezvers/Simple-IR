/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Parameters/Globals.h"
#include "Library/LowHighCutFilters.h"
#include "Library/ProcessorGroup.h"

//==============================================================================
/**
*/
class PluginAudioProcessor  : public juce::AudioProcessor
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


    ProcessorGroup procGroup[3] = { ProcessorGroup("0", true), ProcessorGroup("1", false), ProcessorGroup("2", false) };
    juce::AudioProcessorValueTreeState valueTreeState{*this, nullptr, "PARAMETERS", createParameterLayout()};
    juce::ValueTree variableTree;
    ProcessorGroup& procOut{ procGroup[0]};
    ProcessorGroup& procLeft{ procGroup[1] };
    ProcessorGroup& procRight{ procGroup[2] };

    juce::dsp::ProcessSpec spec;
private:


    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessor)
};
