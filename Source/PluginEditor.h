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
    // Main
    juce::Slider sliderGainOut;
    juce::Slider sliderMix;//
    SampleDrawer sampleDrawer;
    juce::Slider sliderLowCutOut;
    juce::Slider sliderHiCutOut;
    juce::ImageButton buttonBypassOut;
    juce::ImageButton buttonStereoMode;
    // Channels
    juce::Slider sliderGainIR1;
    juce::Slider sliderGainIR2;
    juce::Slider sliderPanIR1;
    juce::Slider sliderPanIR2;
    juce::Slider sliderDelayIR1;
    juce::Slider sliderDelayIR2;
    juce::Slider sliderLowPassIR1;
    juce::Slider sliderLowPassIR2;
    juce::Slider sliderHiPassIR1;
    juce::Slider sliderHiPassIR2;
    juce::ImageButton buttonBypassIR1;
    juce::ImageButton buttonBypassIR2;
    juce::ImageButton buttonInvertIR1;
    juce::ImageButton buttonInvertIR2;
    FileBrowserSamples fileBrowserIR1;
    FileBrowserSamples fileBrowserIR2;

    
    using nSliderAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    using nButtonAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>;
    // Main
    nSliderAttachment attachmentOutGain;
    nSliderAttachment attachmentMix;//
    nSliderAttachment attachmentLowCutOut;
    nSliderAttachment attachmentHiCutOut;
    nButtonAttachment attachmentBypassOut;
    nButtonAttachment attachmentStereoOut;
    // Channels
    nSliderAttachment attachmentGainIR1;
    nSliderAttachment attachmentGainIR2;
    nSliderAttachment attachmentPanIR1;
    nSliderAttachment attachmentPanIR2;
    nSliderAttachment attachmentDelayIR1;
    nSliderAttachment attachmentDelayIR2;
    nSliderAttachment attachmentLowCutIR1;
    nSliderAttachment attachmentLowCutIR2;
    nSliderAttachment attachmentHiCutIR1;
    nSliderAttachment attachmentHiCutIR2;
    nButtonAttachment attachmentBypassIR1;
    nButtonAttachment attachmentBypassIR2;
    nButtonAttachment attachmentInvertIR1;
    nButtonAttachment attachmentInvertIR2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
