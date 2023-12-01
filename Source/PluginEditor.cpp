/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters/Globals.h"

//==============================================================================
PluginAudioProcessorEditor::PluginAudioProcessorEditor (PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    /*
    // --- Slider input gain ---
    sliderOutputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderOutputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    sliderOutputGain.setSkewFactor(2.0, false);
    sliderOutputGain.setRange(Parameters::gainMin, Parameters::gainMax, 0.01);
    sliderOutputGain.setDoubleClickReturnValue(true, 0.0, juce::ModifierKeys::ctrlModifier);
    sliderOutputGain.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderOutputGain.setTextValueSuffix("dB");
    addAndMakeVisible(sliderOutputGain);

    sliderMix.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderMix.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    sliderMix.setRange(Parameters::panMin, Parameters::panMax, 0.01);
    sliderMix.setDoubleClickReturnValue(true, 0.5, juce::ModifierKeys::ctrlModifier);
    addAndMakeVisible(sliderMix);
    */

    //addAndMakeVisible(sampleDrawer);

    attachmentInputGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, Parameters::outputGainId, sliderOutputGain);
    attachmentMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, Parameters::mixId, sliderMix);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setResizable(true, true);
}

PluginAudioProcessorEditor::~PluginAudioProcessorEditor()
{
}

//==============================================================================
void PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Simple IR", getLocalBounds(), juce::Justification::centredTop, 1.0f);
}

void PluginAudioProcessorEditor::resized()
{
    
}

void PluginAudioProcessorEditor::fileDoubleClicked(const File& file)
{
    if (file.getFileExtension() != ".wav" && file.getFileExtension() != ".aiff") {
        return;
    }
    audioProcessor.setIR1(file);
}


