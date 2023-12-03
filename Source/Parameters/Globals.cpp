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

const double gainMin = -125.0;
const double gainMax = 30.0;
const double panMin = 0.0;
const double panMax = 1.0;


const juce::String id_gainOut = "outGain";
const juce::String name_gainOut = "Output Gain";
const juce::String id_mix = "mixID";
const juce::String name_mix = "Mix";
const juce::String id_bypass = "bypassID";
const juce::String name_bypass = "Bypass";

const juce::String param_stereo = "stereoID";
const juce::String param_file1 = "file1";
const juce::String param_file2 = "file2";
const juce::String param_file1Directory = "file1directory";
const juce::String param_file2Directory = "file1directory";

const juce::String lowCutOutID = "lowCutOutID";
const juce::String lowCutOutName = "Low Cut";
const juce::String hiCutOutID = "HiCutOutID";
const juce::String hiCutOutName = "Hi Cut";
const juce::String lowCutIR1ID = "lowCutIR1ID";
const juce::String lowCutIR1Name = "Low Cut";
const juce::String hiCutIR1ID = "HiCutIR1ID";
const juce::String hiCutIR1Name = "Hi Cut";
const juce::String lowCutIR2ID = "lowCutIR2ID";
const juce::String lowCutIR2Name = "Low Cut";
const juce::String hiCutIR2ID = "HiCutIR2ID";
const juce::String hiCutIR2Name = "Hi Cut";


}
