/*
  ==============================================================================

    Globals.h
    Created: 26 Nov 2023 12:20:32pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
namespace Parameters {

// EXAMPLE
extern const juce::String parameterId;
extern const juce::String parameterName;

extern const juce::String variableTreeName;
extern const int fileChooserFlags;
extern const juce::String fileSampleExtensions;

extern const float gainMin;
extern const float gainMax;
extern const float panMin;
extern const float panMax;
extern const float boolMin;
extern const float boolMax;
extern const float freqMin;
extern const float freqMax;
extern const float delayMin;
extern const float delayMax;
extern const juce::AudioParameterBoolAttributes buttonAtributes;
enum enumStereo { MONO, STEREO, DUAL_MONO, COUNT };

}
