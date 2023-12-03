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
extern const float freqMin;
extern const float freqMax;
extern const float delayMin;
extern const float delayMax;
extern const juce::AudioParameterBoolAttributes buttonAtributes;
enum enumStereo { MONO, STEREO, DUAL_MONO, COUNT };

extern const juce::String param_stereoMode;
extern const juce::String param_file1;
extern const juce::String param_file2;
extern const juce::String param_file1Directory;
extern const juce::String param_file2Directory;

extern const juce::String id_mix;
extern const juce::String name_mix;

extern const juce::String id_gainOut;
extern const juce::String name_gainOut;
extern const juce::String id_gainIR1;
extern const juce::String name_gainIR1;
extern const juce::String id_gainIR2;
extern const juce::String name_gainIR2;

extern const juce::String id_bypassOut;
extern const juce::String name_bypassOut;
extern const juce::String id_bypassIR1;
extern const juce::String name_bypassIR1;
extern const juce::String id_bypassIR2;
extern const juce::String name_bypassIR2;

extern const juce::String id_invertIR1;
extern const juce::String name_invertIR1;
extern const juce::String id_invertIR2;
extern const juce::String name_invertIR2;

extern const juce::String id_lowCutOut;
extern const juce::String name_lowCutOut;
extern const juce::String id_lowCutIR1;
extern const juce::String name_lowCutIR1;
extern const juce::String id_lowCutIR2;
extern const juce::String name_lowCutIR2;

extern const juce::String id_hiCutOut;
extern const juce::String name_hiCutOut;
extern const juce::String id_hiCutIR1;
extern const juce::String name_hiCutIR1;
extern const juce::String id_hiCutIR2;
extern const juce::String name_hiCutIR2;

extern const juce::String id_panIR1;
extern const juce::String name_panIR1;
extern const juce::String id_panIR2;
extern const juce::String name_panIR2;

extern const juce::String id_delayIR1;
extern const juce::String name_delayIR1;
extern const juce::String id_delayIR2;
extern const juce::String name_delayIR2;

}
