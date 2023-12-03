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

extern const double gainMin;
extern const double gainMax;
extern const double panMin;
extern const double panMax;
extern const juce::AudioParameterBoolAttributes buttonAtributes;
enum enumStereo { MONO, STEREO, DUAL_MONO, COUNT };

extern const juce::String id_gainOut;
extern const juce::String name_gainOut;
extern const juce::String id_mix;
extern const juce::String name_mix;
extern const juce::String id_bypass;
extern const juce::String name_bypass;

extern const juce::String param_stereo;
extern const juce::String param_file1;
extern const juce::String param_file2;
extern const juce::String param_file1Directory;
extern const juce::String param_file2Directory;


}
