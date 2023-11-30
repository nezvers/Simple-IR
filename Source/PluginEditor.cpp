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
    : AudioProcessorEditor (&p), audioProcessor (p),
    fileFilter(Parameters::fileSampleExtensions, "*", "Choose File"),
    fileBrowser(Parameters::fileChooserFlags, audioProcessor.currentFile, &fileFilter, NULL)
{
    // --- Slider input gain ---
    sliderInputGain.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderInputGain.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    sliderInputGain.setSkewFactor(2.0, false);
    sliderInputGain.setRange(Parameters::gainMin, Parameters::gainMax, 0.01);
    sliderInputGain.setDoubleClickReturnValue(true, 0.0, juce::ModifierKeys::ctrlModifier);
    sliderInputGain.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderInputGain.setTextValueSuffix("dB");
    addAndMakeVisible(sliderInputGain);

    sliderMix.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sliderMix.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
    sliderMix.setRange(Parameters::panMin, Parameters::panMax, 0.01);
    sliderMix.setDoubleClickReturnValue(true, 0.5, juce::ModifierKeys::ctrlModifier);
    addAndMakeVisible(sliderMix);

    //addAndMakeVisible(sampleDrawer);
    fileBrowser.addListener(this);
    addAndMakeVisible(fileBrowser);


    attachmentInputGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, Parameters::outputGainId, sliderInputGain);
    attachmentMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, Parameters::mixId, sliderMix);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    setResizable(true, true);
    DBG("EDITOR INIT");
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
    int leftMargin = getWidth() * 0.02;
    int topMargin = getHeight() * 0.05;
    int rightMargin = getWidth() - getWidth() * 0.05;
    int bottomMargin = getHeight() - getHeight() * 0.05;
    int width = getWidth() * 0.15;
    int height = getHeight() * 0.75 - topMargin * 3;
    sliderInputGain.setBounds(leftMargin, topMargin, width, height);
    int leftMix = leftMargin + width + leftMargin;
    sliderMix.setBounds(leftMix, topMargin, width, height);

    int leftSample = leftMix + width + leftMargin;
    int widthSample = getWidth() - leftSample - leftMargin;
    //sampleDrawer.setBounds(leftMix + width + leftMargin, topMargin, widthSample, getHeight() - topMargin * 2);
    fileBrowser.setBounds(leftMix + width + leftMargin, topMargin, widthSample, getHeight() - topMargin * 2);
}

void PluginAudioProcessorEditor::fileDoubleClicked(const File& file)
{
    DBG(file.getFileName());
    if (file.getFileExtension() != ".wav" && file.getFileExtension() != ".aiff") {
        return;
    }
    audioProcessor.setFile(file);
}


