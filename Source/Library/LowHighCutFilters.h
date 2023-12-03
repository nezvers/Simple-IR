/*
  ==============================================================================

    LowHighCutFilters.h
    Created: 3 Dec 2023 9:34:19pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>

class LowHighCutFilters {
public:
    enum FilterType {LOWCUT, HIGHCUT};
    FilterType type;
    float filterFrequency;
    float sampleRate;

    void process(juce::AudioBuffer<float>& audioBuffer, juce::MidiBuffer midiBuffer);
private:
    std::vector<float> dnBuffer;
};