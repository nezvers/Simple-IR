/*
  ==============================================================================

    SvgUtility.h
    Created: 16 Dec 2023 12:35:21am
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace utility {
    inline void svgSetColour(juce::XmlElement& svg, juce::Colour colour) {
        for (juce::XmlElement* node_0 : svg.getChildIterator()) {
            String style = node_0->getStringAttribute("style");
            DBG(style);
        }
    }
}