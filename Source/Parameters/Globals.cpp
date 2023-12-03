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

const float gainMin = -125.0;
const float gainMax = 30.0;
const float panMin = 0.0;
const float panMax = 1.0;
const float boolMin = 0.0;
const float boolMax = 1.0;
const float freqMin = 20.0;
const float freqMax = 20000.0;
const float delayMin = 0.0f;
const float delayMax = 512.0f;


const juce::String id_mix = "mix";
const juce::String name_mix = "Mix";

const juce::String param_stereoMode = "stereo";
const juce::String param_file1 = "file1";
const juce::String param_file2 = "file2";
const juce::String param_file1Directory = "file1directory";
const juce::String param_file2Directory = "file1directory";

const juce::String id_gainOut = "gainOut";
const juce::String name_gainOut = "Output Gain";
const juce::String id_gainIR1 = "gainIR1";
const juce::String name_gainIR1 = "Gain";
const juce::String id_gainIR2 = "gainIR2";
const juce::String name_gainIR2 = "Gain";

const juce::String id_bypassOut = "bypassOut";
const juce::String name_bypassOut = "Bypass";
const juce::String id_bypassIR1 = "bypassIR1";
const juce::String name_bypassIR1 = "Bypass";
const juce::String id_bypassIR2 = "bypassIR2";
const juce::String name_bypassIR2 = "Bypass";

const juce::String id_invertIR1 = "invertIR1";
const juce::String name_invertIR1 = "Phase Invert";
const juce::String id_invertIR2 = "invertIR2";
const juce::String name_invertIR2 = "Phase Invert";

const juce::String id_lowCutOut = "lowCutOut";
const juce::String name_lowCutOut = "Low Cut";
const juce::String id_lowCutIR1 = "lowCutIR1";
const juce::String name_lowCutIR1 = "Low Cut";
const juce::String id_lowCutIR2 = "lowCutIR2";
const juce::String name_lowCutIR2 = "Low Cut";

const juce::String id_hiCutOut = "HiCutOut";
const juce::String name_hiCutOut = "Hi Cut";
const juce::String id_hiCutIR1 = "HiCutIR1";
const juce::String name_hiCutIR1 = "Hi Cut";
const juce::String id_hiCutIR2 = "HiCutIR2";
const juce::String name_hiCutIR2 = "Hi Cut";

const juce::String id_panIR1 = "panIR1";
const juce::String name_panIR1 = "Pan";
const juce::String id_panIR2 = "panIR2";
const juce::String name_panIR2 = "Pan";

const juce::String id_delayIR1 = "delayIR1";
const juce::String name_delayIR1 = "Delay";
const juce::String id_delayIR2 = "delayIR2";
const juce::String name_delayIR2 = "Delay";


}
