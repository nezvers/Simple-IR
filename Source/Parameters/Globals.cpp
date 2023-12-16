/*
  ==============================================================================

    Globals.cpp
    Created: 26 Nov 2023 12:20:32pm
    Author:  NeZvers

  ==============================================================================
*/

#include "Globals.h"

namespace Parameters {
const juce::String variableTreeName = "Variables";

// EXAMPLE
const juce::String parameterId = "parameter";
const juce::String parameterName = "Parameter";

const int fileChooserFlags = juce::FileBrowserComponent::canSelectFiles | 
    juce::FileBrowserComponent::canSelectDirectories | juce::FileBrowserComponent::openMode;
const juce::String fileSampleExtensions = "*.wav;*.aiff";

const juce::AudioParameterBoolAttributes buttonAtributes = AudioParameterBoolAttributes().withStringFromValueFunction([](auto x, auto) { return x ? "On" : "Off"; })
.withLabel("enabled");

const float gainMin = -125.0f;
const float gainMax = 30.0f;
const float panMin = -1000.0f;
const float panMax = 100.0f;
const float boolMin = 0.0f;
const float boolMax = 1.0f;
const float freqMin = 20.0f;
const float freqMax = 20000.0f;
const float delayMin = 0.0f;
const float delayMax = 512.0f;

}
