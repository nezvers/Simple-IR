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
    init_buttons();


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
        for (int i = 0; i < 6; i++) {
            juce::Slider& slider = *sliderList[i];
            slider.setLookAndFeel(nullptr);
        }

        juce::Button* buttonList[] = {
                &proc->buttonBypass,
                &proc->buttonInvert,
                &proc->buttonStereoMode
        };
        for (int i = 0; i < 3; i++) {
            juce::Button& button = *buttonList[i];
            button.setLookAndFeel(nullptr);
            addAndMakeVisible(button);
        }
    }
    delete lookAndFeel;
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
    int kx_2 = middle - kw / 2;
    int ky_2 = height * ((8.5f) * 0.1f);
    int ftr = height * ((0.5f) * 0.1f);
    int btm = height * ((1.0f) * 0.1f);

    auto localBounds = getLocalBounds();
    auto footerBounds = localBounds.removeFromBottom(ftr);
    auto bottomBounds = localBounds.removeFromBottom(btm);
    auto leftKnobBounds = localBounds.removeFromLeft(kw);
    auto rightKnobBounds = localBounds.removeFromRight(kw);
    auto leftFileBounds = localBounds.removeFromLeft((localBounds.getWidth() - kw) / 2);
    auto rightFileBounds = localBounds.removeFromRight((localBounds.getWidth() - kw));
    auto centerKnobBounds = localBounds;



    DBG(juce::String(middle) + ", " + juce::String(height));

    leftFileBounds.setWidth(juce::jmax(leftFileBounds.getWidth(), 82));
    rightFileBounds.setWidth(juce::jmax(leftFileBounds.getWidth(), 82));
    fileBrowserIR1.setBounds(leftFileBounds);
    fileBrowserIR2.setBounds(rightFileBounds);

    int kh = leftKnobBounds.getHeight() / 4.3f;
    int btw = leftKnobBounds.getWidth() / 2;
    audioProcessor.procLeft.sliderGain.setBounds(leftKnobBounds.removeFromTop(kh));
    audioProcessor.procLeft.sliderMix.setBounds(leftKnobBounds.removeFromTop(kh));
    audioProcessor.procLeft.sliderLowCut.setBounds(leftKnobBounds.removeFromTop(kh));
    audioProcessor.procLeft.sliderHighCut.setBounds(leftKnobBounds.removeFromTop(kh));
    audioProcessor.procLeft.buttonBypass.setBounds(leftKnobBounds.removeFromLeft(btw));
    audioProcessor.procLeft.buttonInvert.setBounds(leftKnobBounds);

    audioProcessor.procRight.sliderGain.setBounds(rightKnobBounds.removeFromTop(kh));
    audioProcessor.procRight.sliderMix.setBounds(rightKnobBounds.removeFromTop(kh));
    audioProcessor.procRight.sliderLowCut.setBounds(rightKnobBounds.removeFromTop(kh));
    audioProcessor.procRight.sliderHighCut.setBounds(rightKnobBounds.removeFromTop(kh));
    audioProcessor.procRight.buttonBypass.setBounds(rightKnobBounds.removeFromLeft(btw));
    audioProcessor.procRight.buttonInvert.setBounds(rightKnobBounds);

    audioProcessor.procOut.sliderGain.setBounds(centerKnobBounds.removeFromTop(kh));
    audioProcessor.procOut.sliderMix.setBounds(centerKnobBounds.removeFromTop(kh));
    audioProcessor.procOut.sliderLowCut.setBounds(centerKnobBounds.removeFromTop(kh));
    audioProcessor.procOut.sliderHighCut.setBounds(centerKnobBounds.removeFromTop(kh));

    int btm_w = bottomBounds.getWidth() / 4;



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
            slider.setLookAndFeel(lookAndFeel);
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

void PluginAudioProcessorEditor::init_buttons()
{
    ProcessorGroup* proc_list[] = { &audioProcessor.procLeft, &audioProcessor.procRight, &audioProcessor.procOut };
    for each (ProcessorGroup * proc in proc_list)
    {
        juce::TextButton* buttonList[] = {
                &proc->buttonBypass,
                & proc->buttonInvert,
                &proc->buttonStereoMode
        };

        const juce::String textList[] = {
            "Solo",
            "Phase Invert",
            "Stereo Mode"
        };

        enum {BYPASS, INVERT, STEREOMODE, COUNT};
        for (int i = 0; i < COUNT; i++) {
            juce::TextButton& button = *buttonList[i];
            button.setLookAndFeel(lookAndFeel);
            button.setButtonText(textList[i]);
            addAndMakeVisible(button);
        }

    }
}


