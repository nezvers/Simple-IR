/*
  ==============================================================================

    ButtonSvg.h
    Created: 14 Dec 2023 8:13:28pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "../Library/SvgUtility.h"

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
        const bool toggle = getToggleState();
        juce::Rectangle<int> b = getLocalBounds();


        {
            // BACKGROUND
            g.setColour(toggle ? colorBgOn : colorBgOff);
            g.fillRoundedRectangle(b.toFloat(), 5.0f);
        }

        {
            // OUTLINE
            g.setColour(juce::Colours::white.withAlpha(hover ? 1.0f : 0.7f));
            g.drawRoundedRectangle(b.toFloat(), 5.0f, 1.0f);
        }
        
        {
            // ICON
            const std::unique_ptr<juce::Drawable> drawable = juce::Drawable::createFromSVG(*svg);
            //utility::svgSetColour(*svg, juce::Colours::greenyellow);
            drawable->replaceColour(juce::Colours::white, toggle ? colorIconOn : colorIconOff);
            drawable->setTransformToFit(b.expanded(-5, -5).toFloat(), RectanglePlacement::centred);
            drawable->draw(g, hover ? 1.0f : 0.7f);
        }
    };

    void setSvg(const char* asset) {
        svg = XmlDocument::parse(asset);
    };

    std::unique_ptr<juce::XmlElement> svg;
    juce::Colour colorIconOn = juce::Colours::orangered;
    juce::Colour colorIconOff = juce::Colours::greenyellow;
    juce::Colour colorBgOn = juce::Colours::black.withAlpha(0.9f);
    juce::Colour colorBgOff = juce::Colours::black.withAlpha(0.1f);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonSvg)
};
