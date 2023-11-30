/*
  ==============================================================================

    FileLoader.h
    Created: 29 Nov 2023 11:54:27am
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace FileLoad {
    void Sample(juce::String& path, juce::AudioBuffer<float> &buffer) {
        juce::File file = File(path);
        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();
        juce::AudioFormatReader* formatReader = formatManager.createReaderFor(file);

        int sample_length = static_cast<int>(formatReader->lengthInSamples);

        buffer.setSize(2, sample_length); // TODO: don't assume stereo
        formatReader->read(&buffer, 0, sample_length, 0, true, true);
    }
}