/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/SampleDrawer.h"
#include "Components/FileBrowserSamples.h"
#include "Library/CustomLookAndFeel.h"

//==============================================================================
/**
*/
class PluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PluginAudioProcessorEditor (PluginAudioProcessor&);
    ~PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    PluginAudioProcessor& audioProcessor;
    
    SampleDrawer sampleDrawer;
    
    FileBrowserSamples fileBrowserIR1;
    FileBrowserSamples fileBrowserIR2;


    juce::Rectangle<int> localBounds;
    juce::Rectangle<int> footerBounds;
    //juce::Rectangle<int> bottomBounds;
    juce::Rectangle<int> leftKnobBounds;
    juce::Rectangle<int> rightKnobBounds;
    juce::Rectangle<int> leftFileBounds;
    juce::Rectangle<int> rightFileBounds;
    juce::Rectangle<int> centerKnobBounds;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
