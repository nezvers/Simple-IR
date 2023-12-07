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
    ProcessorGroup* proc_list[] = { &audioProcessor.procLeft, &audioProcessor.procRight, &audioProcessor.procOut };

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
        enum { MIX, GAIN, LOWCUT, HIGHCUT, PAN, DELAY, COUNT };
        for (int i = 0; i < COUNT; i++) {
            juce::Slider& slider = *sliderList[i];
            slider.setLookAndFeel(nullptr);
        }
    }
    delete slickLookAndFeel;
}

//==============================================================================
void PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.fillAll(juce::Colour::fromRGB(0.1f, 0.1f, 0.1f));
    g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Simple IR", getLocalBounds(), juce::Justification::centredTop, 1.0f);
}

void PluginAudioProcessorEditor::resized()
{
    int width = getWidth();
    int height = getHeight();
    int middle = width * 0.5;
    
    int y_2 = height * ((5.0f) * 0.1f);
    int h_1 = height * ((2.0f) * 0.1f);

    int fx = width * ((1.0f) * 0.1f);
    int fy = height * ((0.25f) * 0.1f);
    int fw = width * ((4.0f) * 0.1f);
    int fm = width * ((0.5f) * 0.1f);
    int fh = height * ((8.0f) * 0.1f);

    int kx_1 = width * ((0.0f) * 0.1f);
    int ky = height * ((0.2f) * 0.1f);
    int kw = width * ((1.1f) * 0.1f);
    int kh = height * ((1.5f) * 0.1f);
    int kx_2 = middle - kw / 2;
    int ky_2 = height * ((8.5f) * 0.1f);


    DBG(juce::String(middle) + ", " + juce::String(height));

    int fileBrowserWidth = juce::jmax(fw - fm, 82);
    fileBrowserIR1.setBounds(fx, fy, fileBrowserWidth, fh);
    fileBrowserIR2.setBounds(middle + fm, fy, fileBrowserWidth, fh);

    audioProcessor.procLeft.sliderGain.setBounds(kx_1, ky, kw, kh);
    audioProcessor.procRight.sliderGain.setBounds(width - kx_1 - kw, ky, kw, kh);

    audioProcessor.procLeft.sliderMix.setBounds(kx_1, ky + kh, kw, kh);
    audioProcessor.procRight.sliderMix.setBounds(width - kx_1 - kw, ky + kh, kw, kh);

    audioProcessor.procLeft.sliderLowCut.setBounds(kx_1, ky + kh * 2, kw, kh);
    audioProcessor.procRight.sliderLowCut.setBounds(width - kx_1 - kw, ky + kh * 2, kw, kh);

    audioProcessor.procLeft.sliderHighCut.setBounds(kx_1, ky + kh * 3, kw, kh);
    audioProcessor.procRight.sliderHighCut.setBounds(width - kx_1 - kw, ky + kh * 3, kw, kh);

    audioProcessor.procOut.sliderGain.setBounds(kx_2, ky + kh * 0, kw, kh);
    audioProcessor.procOut.sliderMix.setBounds(kx_2, ky + kh * 1, kw, kh);

    audioProcessor.procOut.sliderLowCut.setBounds(kx_2, ky + kh * 2, kw, kh);
    audioProcessor.procOut.sliderHighCut.setBounds(kx_2, ky + kh * 3, kw, kh);

}

void PluginAudioProcessorEditor::init_sliders()
{
    ProcessorGroup* proc_list[] = { &audioProcessor.procLeft, &audioProcessor.procRight, &audioProcessor.procOut };
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

        const float skewList[] = {
            1.0f,
            2.0f,
            10.0f,
            10.0f,
            1.0f,
            0.5f
        };

        const float returnList[] = {
            1.0f,
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
            slider.setLookAndFeel(slickLookAndFeel);
            slider.setName(nameList[i]);
            slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 72, 32);
            slider.setSkewFactor(skewList[i], false);
            slider.setRange(minList[i], maxList[i], 0.01);
            slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
            slider.setDoubleClickReturnValue(true, returnList[i], juce::ModifierKeys::ctrlModifier);
            slider.setTextValueSuffix(suffixList[i]);
            addAndMakeVisible(slider);
        }
    }
}


