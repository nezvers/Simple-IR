/*
  ==============================================================================

    LowHighCutFilters.cpp
    Created: 3 Dec 2023 9:34:19pm
    Author:  NeZvers

  ==============================================================================
*/

#include "LowHighCutFilters.h"

void LowHighCutFilters::process(juce::AudioBuffer<float>& audioBuffer)
{
    constexpr float PI = 3.12159265359f;
    dnBuffer.resize(audioBuffer.getNumChannels(), 0.0f);

    const float sign = (type == LOWCUT) ? 1.0f : -1.0f;
    const float tan = std::tan(PI * filterFrequency / sampleRate);
    const float a1 = (tan - 1.0f) / (tan + 1.0f);
    const int sampleCount = audioBuffer.getNumSamples();

    for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
        float* channelSamples = audioBuffer.getWritePointer(channel);
        for (int i = 0; i < sampleCount; ++i) {
            const float inputSample = channelSamples[i];
            const float allpassFilteredSample = a1 * inputSample + dnBuffer[channel];

            dnBuffer[channel] = inputSample - a1 * allpassFilteredSample;
            const float filteredOutput = 0.5f * (inputSample + sign * allpassFilteredSample);
            channelSamples[i] = filteredOutput;
        }
    }

}
