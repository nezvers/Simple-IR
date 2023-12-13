/*
  ==============================================================================

    Components.h
    Created: 12 Dec 2023 11:06:12pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace comp {
    static constexpr float tau = 6.28318530718f;

    struct Comp :public juce::Component {
        Comp():
            juce::Component()
        {
            setBufferedToImage(true);
        }

        juce::String tooltip;

    };
}