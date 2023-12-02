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

class FileBrowserSamples : public juce::Component, public juce::FileBrowserListener
{
public:
    FileBrowserSamples():
        fileBrowser(juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles |
            juce::FileBrowserComponent::canSelectDirectories,
            juce::File::getSpecialLocation(juce::File::userDocumentsDirectory), 
            &fileFilter, NULL)
    {
        fileBrowser.addListener(this);
        addAndMakeVisible(fileBrowser);
    };

    ~FileBrowserSamples() {};
    void fileDoubleClicked(const File& file) override {
        if (onFileDoubleClick != nullptr) {
            onFileDoubleClick(file);
        }
    };

    void resized() override { fileBrowser.setBounds(getLocalBounds()); };

    void paint(juce::Graphics&) override {};
    void selectionChanged() override {};
    void fileClicked(const File& file, const MouseEvent& e) override {};
    void browserRootChanged(const File& newRoot) override {};

    std::function<void(juce::File)> onFileDoubleClick;

private:
    juce::WildcardFileFilter fileFilter{ Parameters::fileSampleExtensions, "*", "Choose File" };
    juce::FileBrowserComponent fileBrowser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileBrowserSamples)
};
