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

    float height = 545.f;
    setSize (840, int(height));
    setResizable(true, false);
    //setResizeLimits(100, 100, 9999, 9999);
    getConstrainer()->setFixedAspectRatio(840.f / height);

    for each (Component * component in audioProcessor.procLeft.getComponentList()) {
        addAndMakeVisible(component);
    }
    for each (Component * component in audioProcessor.procRight.getComponentList()) {
        addAndMakeVisible(component);
    }
    for each (Component * component in audioProcessor.procOut.getComponentList()) {
        addAndMakeVisible(component);
    }
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
    //g.setColour (juce::Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Simple IR", getLocalBounds(), juce::Justification::centredTop, 1.0f);
    float g1 = 0.1f;
    float g2 = 0.2f;
    float g3 = 0.3f;
    float g4 = 0.4f;
    g.setColour(juce::Colour::fromFloatRGBA(g3, g3, g3, 1.0f));
    g.fillRect(leftKnobBounds.getX(), leftKnobBounds.getY(), leftKnobBounds.getWidth(), leftKnobBounds.getHeight());
    g.fillRect(rightKnobBounds.getX(), rightKnobBounds.getY(), rightKnobBounds.getWidth(), rightKnobBounds.getHeight());
    g.fillRect(centerKnobBounds.getX(), centerKnobBounds.getY(), centerKnobBounds.getWidth(), centerKnobBounds.getHeight());

    g.setColour(juce::Colour::fromFloatRGBA(g2, g2, g2, 1.0f));
    g.fillRect(leftFileBounds.getX(), leftFileBounds.getY(), leftFileBounds.getWidth(), leftFileBounds.getHeight());
    g.fillRect(rightFileBounds.getX(), rightFileBounds.getY(), rightFileBounds.getWidth(), rightFileBounds.getHeight());

    //g.setColour(juce::Colour::fromFloatRGBA(g4, g4, g4, 1.0f));
    //g.fillRect(bottomBounds.getX(), bottomBounds.getY(), bottomBounds.getWidth(), bottomBounds.getHeight());

    g.setColour(juce::Colour::fromFloatRGBA(g1, g1, g1, 1.0f));
    g.fillRect(footerBounds.getX(), footerBounds.getY(), footerBounds.getWidth(), footerBounds.getHeight());
}

void PluginAudioProcessorEditor::resized()
{
    int width = getWidth();
    int height = getHeight();
    int middle = width * 0.5;
    DBG(juce::String(width) + ", " + juce::String(height));// +" - aspect: " + juce::String(float(width) / float(height)));
    
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
    int ftr = height * ((0.84f) * 0.1f);
    int btm = height * ((1.0f) * 0.1f);

    localBounds = getLocalBounds();
    auto lb = localBounds;
    footerBounds = lb.removeFromBottom(ftr);
    //bottomBounds = lb.removeFromBottom(btm);
    leftKnobBounds = lb.removeFromLeft(kw);
    rightKnobBounds = lb.removeFromRight(kw);
    leftFileBounds = lb.removeFromLeft((lb.getWidth() - kw) / 2);
    rightFileBounds = lb.removeFromRight(lb.getWidth() - kw);
    centerKnobBounds = lb;

    auto _footerBounds = footerBounds;
    //auto _bottomBounds = bottomBounds;
    auto _leftKnobBounds = leftKnobBounds;
    auto _rightKnobBounds = rightKnobBounds;
    auto _leftFileBounds = leftFileBounds;
    auto _rightFileBounds = rightFileBounds;
    auto _centerKnobBounds = centerKnobBounds;


    _leftFileBounds.setWidth(juce::jmax(_leftFileBounds.getWidth(), 82));
    _rightFileBounds.setWidth(juce::jmax(_leftFileBounds.getWidth(), 82));
    fileBrowserIR1.setBounds(_leftFileBounds);
    fileBrowserIR2.setBounds(_rightFileBounds);

    int kh = leftKnobBounds.getHeight() / 6;
    int btw = leftKnobBounds.getWidth() / 2;
    audioProcessor.procLeft.sliderGain.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procLeft.sliderMix.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procLeft.sliderLowCut.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procLeft.sliderHighCut.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procLeft.sliderDelay.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procLeft.sliderPan.setBounds(_leftKnobBounds.removeFromTop(kh).expanded(-5, -5));

    audioProcessor.procRight.sliderGain.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procRight.sliderMix.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procRight.sliderLowCut.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procRight.sliderHighCut.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procRight.sliderDelay.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procRight.sliderPan.setBounds(_rightKnobBounds.removeFromTop(kh).expanded(-5, -5));

    audioProcessor.procOut.sliderGain.setBounds(_centerKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procOut.sliderMix.setBounds(_centerKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procOut.sliderLowCut.setBounds(_centerKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procOut.sliderHighCut.setBounds(_centerKnobBounds.removeFromTop(kh).expanded(-5, -5));
    audioProcessor.procOut.sliderPan.setBounds(_centerKnobBounds.removeFromTop(kh).expanded(-5, -5));

    
    auto ftr_1 = _footerBounds.removeFromLeft(kw);
    audioProcessor.procLeft.buttonBypass.setBounds(ftr_1.removeFromLeft(ftr_1.getWidth() / 2).expanded(-5, -5));
    audioProcessor.procLeft.buttonInvert.setBounds(ftr_1.expanded(-5, -5));

    auto ftr_2 = _footerBounds.removeFromLeft(_leftFileBounds.getWidth());
    auto ftr_3 = _footerBounds.removeFromLeft(kw);
    audioProcessor.procOut.buttonBypass.setBounds(ftr_3.removeFromLeft(ftr_3.getWidth() / 2).expanded(-5, -5));
    audioProcessor.procOut.buttonInvert.setBounds(ftr_3.expanded(-5, -5));

    auto ftr_4 = _footerBounds.removeFromLeft(_rightFileBounds.getWidth());;
    audioProcessor.procOut.comboStereoMode.setBounds(ftr_4.removeFromLeft(ftr_4.getWidth() / 2).expanded(-5, -5));
    
    auto ftr_5 = _footerBounds;
    audioProcessor.procRight.buttonBypass.setBounds(ftr_5.removeFromLeft(ftr_5.getWidth() / 2).expanded(-5, -5));

}



