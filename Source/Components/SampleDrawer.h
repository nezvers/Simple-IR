/*
  ==============================================================================

    SampleDrawer.h
    Created: 29 Nov 2023 2:14:38am
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SampleDrawer : public juce::Component {
public:
    SampleDrawer() {};
    ~SampleDrawer() {};
    /*void resized() override {

    };*/
    void paint(juce::Graphics& g) override{
        float width = getWidth();
        float height = getHeight();
        g.setColour(juce::Colours::whitesmoke);
        g.drawLine(0.0f, 0.0f, width, height);
    };
    void drawSample(juce::AudioBuffer<float>* newSample) {
        sample = newSample;
        repaint();
    }
private:
    juce::AudioBuffer<float>* sample = nullptr;
    // TODO: zooming like Fenrir
};