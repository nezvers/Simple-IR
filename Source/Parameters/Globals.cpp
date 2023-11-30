/*
  ==============================================================================

    Globals.cpp
    Created: 26 Nov 2023 12:20:32pm
    Author:  NeZvers

  ==============================================================================
*/

#include "Globals.h"

namespace Parameters {

// EXAMPLE
const juce::String parameterId = "parameter";
const juce::String parameterName = "Parameter";

const double gainMin = -125.0;
const double gainMax = 30.0;

const double panMin = 0.0;
const double panMax = 1.0;

const juce::String inputGainId = "inputGain";
const juce::String inputGainName = "Input Gain";

const juce::String mixId = "mixID";
const juce::String mixName = "mixID";


const juce::String irFileId = "ir_file";
const juce::String variableTreeName = "Variables";
const juce::String file1 = "file1";
const juce::String root = "root";

const int fileChooserFlags = juce::FileBrowserComponent::canSelectFiles |
juce::FileBrowserComponent::canSelectDirectories |
juce::FileBrowserComponent::openMode;
const juce::String fileExtensions = "*wav;*aiff";
}
