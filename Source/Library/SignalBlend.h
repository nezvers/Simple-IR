/*
  ==============================================================================

    SignalBlend.h
    Created: 18 Dec 2023 12:32:58am
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class SignalBlend {
public:
    SignalBlend() {};
    ~SignalBlend() {};

    void blend(
        juce::AudioBuffer<float> a, 
        ::AudioBuffer<float> b, 
        std::atomic<float>* value)
    {
        buffer.makeCopyOf(b, true);
        juce::dsp::AudioBlock<float> audioBlock = { buffer };
        juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    };

    juce::AudioBuffer<float> buffer;
};