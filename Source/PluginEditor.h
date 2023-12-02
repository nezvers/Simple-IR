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
    juce::Slider sliderOutputGain;
    juce::Slider sliderMix;
    SampleDrawer sampleDrawer;
    FileBrowserSamples fileBrowser1;
    FileBrowserSamples fileBrowser2;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentInputGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentMix;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
