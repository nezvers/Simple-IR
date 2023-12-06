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
    init_sliders();
    


    //addAndMakeVisible(sampleDrawer);
    DBG("_PluginAudioProcessorEditor()");
    fileBrowserIR1.setSelectedFile(audioProcessor.procLeft.file, audioProcessor.procLeft.directory);
    fileBrowserIR2.setSelectedFile(audioProcessor.procRight.file, audioProcessor.procRight.directory);
    addAndMakeVisible(fileBrowserIR1);
    addAndMakeVisible(fileBrowserIR2);
    fileBrowserIR1.onFileDoubleClick = [this](juce::File file) 
        {audioProcessor.procLeft.setFile(file); };
    fileBrowserIR2.onFileDoubleClick = [this](juce::File file)
    {audioProcessor.procRight.setFile(file); };


    setSize (400, 300);
    setResizable(true, true);
    setResizeLimits(100, 100, 9999, 9999);
    //getConstrainer()->setFixedAspectRatio(2.0);
}

PluginAudioProcessorEditor::~PluginAudioProcessorEditor()
{
}

//==============================================================================
void PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.fillAll(juce::Colour::fromRGB(0.1f, 0.1f, 0.1f));
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Simple IR", getLocalBounds(), juce::Justification::centredTop, 1.0f);
}

void PluginAudioProcessorEditor::resized()
{
    int width = getWidth();
    int height = getHeight();
    int middle = width * 0.5;
    DBG(juce::String(middle) + ", " + juce::String(height));

    int fileBrowserWidth = juce::jmax(middle, 82);
    fileBrowserIR1.setBounds(0, 0, fileBrowserWidth, height);
    fileBrowserIR2.setBounds(middle,0, fileBrowserWidth, height);
}

void PluginAudioProcessorEditor::init_sliders()
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

    ProcessorGroup* proc_list[] = { &audioProcessor.procLeft, &audioProcessor.procRight };
    for each (ProcessorGroup * proc in proc_list)
    {
        juce::Slider* sliderList[] = {
            &proc->sliderMix,
            &proc->sliderGain,
            &proc->sliderLowCut,
            &proc->sliderHighCut,
            &proc->sliderPan,
            &proc->sliderDelay
        };
        juce::String nameList[] = {
            proc->name_mix,
            proc->name_gain,
            proc->name_lowCut,
            proc->name_highCut,
            proc->name_pan,
            proc->name_delay
        };
        float minList[] = {
            Parameters::panMin,
            Parameters::gainMin,
            Parameters::freqMin,
            Parameters::freqMin,
            Parameters::panMin,
            Parameters::delayMin
        };
        float maxList[] = {
            Parameters::panMax,
            Parameters::gainMax,
            Parameters::freqMax,
            Parameters::freqMax,
            Parameters::panMax,
            Parameters::delayMax
        };
        const float returnList[] = {
            0.5f,
            0.0f,
            20.0f,
            20000.0f,
            0.5f,
            0.0f
        };
        const juce::String suffixList[] = {
            "",
            "dB",
            "Hz",
            "Hz",
            "",
            ""
        };

        enum {MIX, GAIN, LOWCUT, HIGHCUT, PAN, DELAY, COUNT};
        for (int i = 0; i < COUNT; i++) {
            juce::Slider& slider = *sliderList[i];
            slider.setName(nameList[i]);
            slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
            slider.setSkewFactor(2.0, false);
            slider.setRange(minList[i], maxList[i], 0.01);
            slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
            slider.setDoubleClickReturnValue(true, returnList[i], juce::ModifierKeys::ctrlModifier);
            slider.setTextValueSuffix(suffixList[i]);
            addAndMakeVisible(slider);
        }
    }
}


