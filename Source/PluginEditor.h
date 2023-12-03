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
    
    juce::Slider sliderMix;//
    SampleDrawer sampleDrawer;
    juce::Slider sliderGainOut;
    juce::Slider sliderGainIR1;
    juce::Slider sliderGainIR2;
    juce::ImageButton buttonBypassOut;
    juce::ImageButton buttonStereoMode;
    juce::Slider sliderLowCutOut;
    juce::Slider sliderLowCutIR1;
    juce::Slider sliderLowCutIR2;
    juce::Slider sliderHiCutOut;
    juce::Slider sliderHiCutIR1;
    juce::Slider sliderHiCutIR2;
    juce::ImageButton buttonBypassIR1;
    juce::ImageButton buttonBypassIR2;
    juce::ImageButton buttonInvertIR1;
    juce::ImageButton buttonInvertIR2;
    juce::Slider sliderPanIR1;
    juce::Slider sliderPanIR2;
    juce::Slider sliderDelayIR1;
    juce::Slider sliderDelayIR2;
    FileBrowserSamples fileBrowserIR1;
    FileBrowserSamples fileBrowserIR2;

    
    using nSliderAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    using nButtonAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>;
    
    nSliderAttachment attachmentMix;//
    nSliderAttachment attachmentGainOut;
    nSliderAttachment attachmentGainIR1;
    nSliderAttachment attachmentGainIR2;
    nButtonAttachment attachmentStereoOut;
    // Channels
    nSliderAttachment attachmentLowCutOut;
    nSliderAttachment attachmentLowCutIR1;
    nSliderAttachment attachmentLowCutIR2;
    nSliderAttachment attachmentHiCutOut;
    nSliderAttachment attachmentHiCutIR1;
    nSliderAttachment attachmentHiCutIR2;
    nButtonAttachment attachmentBypassOut;
    nButtonAttachment attachmentBypassIR1;
    nButtonAttachment attachmentBypassIR2;
    nButtonAttachment attachmentInvertIR1;
    nButtonAttachment attachmentInvertIR2;
    nSliderAttachment attachmentPanIR1;
    nSliderAttachment attachmentPanIR2;
    nSliderAttachment attachmentDelayIR1;
    nSliderAttachment attachmentDelayIR2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
