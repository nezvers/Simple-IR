/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/SampleDrawer.h"

//==============================================================================
/**
*/
class PluginAudioProcessorEditor  : public juce::AudioProcessorEditor,
    juce::FileBrowserListener
{
public:
    PluginAudioProcessorEditor (PluginAudioProcessor&);
    ~PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void fileDoubleClicked(const File& file) override;

    void selectionChanged() override {};
    void fileClicked(const File& file, const MouseEvent& e) override {};
    void browserRootChanged(const File& newRoot) override {};
private:


    PluginAudioProcessor& audioProcessor;
    juce::Slider sliderInputGain;
    juce::Slider sliderMix;
    SampleDrawer sampleDrawer;
    
    juce::WildcardFileFilter fileFilter;
    juce::FileBrowserComponent fileBrowser;
    std::unique_ptr<juce::FileChooser> fileChooser;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentInputGain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachmentMix;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginAudioProcessorEditor)
};
