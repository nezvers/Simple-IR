/*
  ==============================================================================

    SliderWithLabel.h
    Created: 13 Dec 2023 11:32:40pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SliderWithLabel : public Slider {
public:
    SliderWithLabel() {
        addAndMakeVisible(label);
        label.setJustificationType(juce::Justification::centred);
        juce::Font font = label.getFont();
        font.setHeight(font.getHeight() * 0.9f);
        label.setFont(font);
        label.setInterceptsMouseClicks(false, false);
    };
    ~SliderWithLabel() {};

    void resized() override {
        Slider::resized();
        juce::Rectangle<int> bounds = getLocalBounds();
        int height = bounds.getHeight();
        bounds = bounds.removeFromBottom(height * 0.2f);
        bounds.setY(height * 0.5f);
        label.setBounds(bounds);
    }
    void paint(juce::Graphics& g) override {
        Slider::paint(g);
    }
    juce::Label label;
};