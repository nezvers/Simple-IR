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
    juce::Slider sliderMix;//
    SampleDrawer sampleDrawer;
    juce::Slider sliderLowPassOut;
    juce::Slider sliderHiPassOut;
    juce::ImageButton buttonBypassOut;
    juce::ImageButton buttonStereo;

    juce::Slider sliderIrGain1;
    juce::Slider sliderIrGain2;
    juce::Slider sliderIrpan1;
    juce::Slider sliderIrpan2;
    juce::Slider sliderIrDelay1;
    juce::Slider sliderIrDelay2;
    juce::Slider sliderIrLowPass1;
    juce::Slider sliderIrLowPass2;
    juce::Slider sliderIrHiPass1;
    juce::Slider sliderIrHiPass2;
    juce::ImageButton buttonIrBypass1;
    juce::ImageButton buttonIrBypass2;
    juce::ImageButton buttonIrInvert1;
    juce::ImageButton buttonIrInvert2;

    FileBrowserSamples fileBrowser1;
    FileBrowserSamples fileBrowser2;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentOutGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentMix;//
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentLowPassOut;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentHiPassOut;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentBypassOut;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentStareoOut;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrGain1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrGain2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrPan1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrPan2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrDelay1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrDelay2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrLowPass1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrLowPass2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrHiPass1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentIrHiPass2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentIrBypass1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentIrBypass2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentIrInvert1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachmentIrInvert2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
