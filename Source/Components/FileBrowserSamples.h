/*
  ==============================================================================

    FileBrowserSamples.h
    Created: 30 Nov 2023 9:49:42pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Parameters/Globals.h"

class FileBrowserSamples : public juce::FileBrowserComponent, juce::FileBrowserListener
{
public:
    FileBrowserSamples(juce::File init_file):
        fileFilter(Parameters::fileSampleExtensions, "*", "Choose File"),
        juce::FileBrowserComponent(Parameters::fileChooserFlags, init_file, &fileFilter, NULL)
    {
        addListener(this);
    };

    ~FileBrowserSamples() {};
    void fileDoubleClicked(const File& file) override {
        if (onFileDoubleClick != nullptr) {
            onFileDoubleClick(file);
        }
    };

    void selectionChanged() override {};
    void fileClicked(const File& file, const MouseEvent& e) override {};
    void browserRootChanged(const File& newRoot) override {};

    std::function<void(juce::File)> onFileDoubleClick;

private:
    juce::WildcardFileFilter fileFilter; // { Parameters::fileExtensions, "*", "Choose File" };
};
