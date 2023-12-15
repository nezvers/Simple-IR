/*
  ==============================================================================

    ButtonSvg.h
    Created: 14 Dec 2023 8:13:28pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class ButtonSvg :public juce::Button {
public:
    ButtonSvg() :Button("") {

    };

    void resized() override {

    };

    void paintButton(Graphics& g, const bool isHighlighted,
        const bool isDown) override {
        jassert(true);
    };

    void paint(juce::Graphics& g) override {
        const bool down = isDown();
        const bool hover = isOver();
        juce::Rectangle<int> b = getLocalBounds();


        {
            g.setColour(juce::Colours::white.withAlpha(hover ? 1.0f : 0.7f));
            g.drawRoundedRectangle(b.getX(), b.getY(), b.getWidth(), b.getHeight(), 5.0f, 1.0f);
        }
        
        {
        const std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVG(*svg);
        drawable->replaceColour(juce::Colours::white, juce::Colours::green);
        drawable->setTransformToFit(b.expanded(-5, -5).toFloat(), RectanglePlacement::centred);
        drawable->draw(g, hover ? 1.0f : 0.7f);
        }
    };

    void setSvg(const char* asset) {
        svg = XmlDocument::parse(asset);
    };

    std::unique_ptr<juce::XmlElement> svg;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonSvg)
};
