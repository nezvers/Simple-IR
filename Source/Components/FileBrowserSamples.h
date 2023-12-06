/*
  ==============================================================================

    FileBrowserSamples.h
    Created: 30 Nov 2023 9:49:42pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

    void setSelectedFile(juce::File file, juce::File directory) {
        if (!file.existsAsFile()) {
            return;
        }
        fileBrowser.setRoot(directory);

        juce::String extension = file.getFileExtension();
        if (extension != ".wav" && extension != ".aiff") { return; }

        fileBrowser.setFileName(file.getFileName());
    };

    void resized() override { fileBrowser.setBounds(getLocalBounds()); };

    void paint(juce::Graphics&) override {};
    void selectionChanged() override {};
    void fileClicked(const File& file, const MouseEvent& e) override {};
    void browserRootChanged(const File& newRoot) override {};

    std::function<void(juce::File)> onFileDoubleClick;
    juce::WildcardFileFilter fileFilter = juce::WildcardFileFilter("*.wav;*.aiff", "", "Choose File" );
    juce::FileBrowserComponent fileBrowser;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileBrowserSamples)
};
