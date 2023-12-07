/*
  ==============================================================================

    ProcessorGroup.h
    Created: 4 Dec 2023 10:12:44pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "LowHighCutFilters.h"
#include "../Parameters/Globals.h"

class ProcessorGroup {
public:
    ProcessorGroup(juce::String idSuffix) {
        suffix = idSuffix;
        DBG("_ProcessorGroup()" + suffix);
        param_file += idSuffix;
        param_directory += idSuffix;
        param_stereoMode += idSuffix;
        id_mix += idSuffix;
        id_gain += idSuffix;
        id_lowCut += idSuffix;
        id_highCut += idSuffix;
        id_pan += idSuffix;
        id_delay += idSuffix;
        id_bypass += idSuffix;
        id_invert += idSuffix;
    };
#pragma region Parameter IDs
    juce::String suffix;
    juce::String param_file = "file";
    juce::String param_directory = "directory";
    juce::String param_stereoMode = "stereo";
    juce::String id_mix = "mix";
    juce::String id_gain = "gain";
    juce::String id_lowCut = "lowCut";
    juce::String id_highCut = "highCut";
    juce::String id_pan = "pan";
    juce::String id_delay = "delay";
    juce::String id_bypass = "bypass";
    juce::String id_invert = "invert";
#pragma endregion

#pragma region Parameter names
    const juce::String name_mix = "Mix";
    const juce::String name_gain = "Gain";
    const juce::String name_lowCut = "Low Cut";
    const juce::String name_highCut = "High Cut";
    const juce::String name_pan = "High Cut";
    const juce::String name_delay = "Delay";
    const juce::String name_bypass = "Bypass";
    const juce::String name_invert = "Phase Invert";
#pragma endregion
#pragma region value links
    std::atomic<float>* valueMix = nullptr;
    std::atomic<float>* valueGain = nullptr;
    std::atomic<float>* valueLowCut= nullptr;
    std::atomic<float>* valueHighCut = nullptr;
    std::atomic<float>* valuePan = nullptr;
    std::atomic<float>* valueDelay = nullptr;
    std::atomic<float>* valueBypass = nullptr;
    std::atomic<float>* valueInvert = nullptr;
#pragma endregion
#pragma region Components
    juce::Slider sliderMix;//
    juce::Slider sliderGain;
    juce::Slider sliderLowCut;
    juce::Slider sliderHighCut;
    juce::Slider sliderPan;
    juce::Slider sliderDelay;
    juce::TextButton buttonBypass;
    juce::TextButton buttonInvert;
    juce::TextButton buttonStereoMode;
#pragma endregion
#pragma region Attachments
    using nSliderAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    using nButtonAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>;

    nSliderAttachment attachmentMix;
    nSliderAttachment attachmentGain;
    nSliderAttachment attachmentLowCut;
    nSliderAttachment attachmentHighCut;
    nSliderAttachment attachmentPan;
    nSliderAttachment attachmentDelay;
    nButtonAttachment attachmentBypass;
    nButtonAttachment attachmentInvert;
#pragma endregion
#pragma region DSP
    juce::dsp::DryWetMixer<float> mix;
    juce::dsp::DryWetMixer<float> pan;
    juce::dsp::Gain<float> gain;
    juce::dsp::Convolution convolution;
    LowHighCutFilters filterLowCut;
    LowHighCutFilters filterHighCut;
    juce::AudioBuffer<float> buffer;
    juce::File file = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    juce::File directory = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    juce::ValueTree* variableTree = nullptr;
    juce::AudioProcessorValueTreeState* valueTreeState = nullptr;
    Parameters::enumStereo stereoMode = Parameters::enumStereo::DUAL_MONO;
#pragma endregion

    // Initializes DSP
    void setSpec(juce::dsp::ProcessSpec& spec) {
        gain.prepare(spec);
        mix.prepare(spec);
        pan.prepare(spec);
        convolution.prepare(spec);
        gain.setRampDurationSeconds(0.01);
        filterLowCut.type = LowHighCutFilters::LOWCUT;
        filterHighCut.type = LowHighCutFilters::HIGHCUT;
    }

    // Links atomic valiables
    void linkParameters(juce::AudioProcessorValueTreeState* vts) {
        DBG("linkParameters()" + suffix);
        valueMix = vts->getRawParameterValue(id_mix);
        valueGain = vts->getRawParameterValue(id_gain);
        valueLowCut = vts->getRawParameterValue(id_lowCut);
        valueHighCut = vts->getRawParameterValue(id_highCut);
        valuePan = vts->getRawParameterValue(id_pan);
        valueDelay = vts->getRawParameterValue(id_delay);
        valueBypass = vts->getRawParameterValue(id_bypass);
        valueInvert = vts->getRawParameterValue(id_invert);
    }

    // Generates ValueTreeState parameters
    void setParameterLayout(std::vector <std::unique_ptr<juce::RangedAudioParameter>>& params) {
        DBG("setParameterLayout()" + suffix);
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_mix, name_mix, Parameters::panMin, Parameters::panMax, 1.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_gain, name_gain, Parameters::gainMin, Parameters::gainMax, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_lowCut, name_lowCut, Parameters::freqMin, Parameters::freqMax, Parameters::freqMin));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_highCut, name_highCut, Parameters::freqMin, Parameters::freqMax, Parameters::freqMax));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_pan, name_pan, Parameters::panMin, Parameters::panMax, 0.5f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_delay, name_delay, Parameters::panMin, Parameters::panMax, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_bypass, name_bypass, Parameters::panMin, Parameters::panMax, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_invert, name_invert, Parameters::panMin, Parameters::panMax, 0.0f));
    }

    void setTreeStateReferences(juce::AudioProcessorValueTreeState* vts) {
        DBG("setTreeStateReferences()" + suffix);
        valueTreeState = vts;

        linkParameters(vts);

        // Connect attachments to GUI components
        attachmentMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_mix, sliderMix);
        attachmentGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_gain, sliderGain);
        attachmentLowCut = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_lowCut, sliderLowCut);
        attachmentHighCut = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_highCut, sliderHighCut);
        attachmentPan = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_pan, sliderPan);
        attachmentDelay = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_delay, sliderDelay);
        attachmentBypass = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*vts, id_bypass, buttonBypass);
        attachmentInvert = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*vts, id_invert, buttonInvert);
    }

    // LOAD
    void setStateInformation(juce::ValueTree* vt) {
        DBG("setStateInformation()" + suffix);
        variableTree = vt;
        
        juce::File fileLoad = juce::File(vt->getProperty(param_file));
        if (!fileLoad.existsAsFile()) {
            return;
        }
        file = juce::File(vt->getProperty(param_file));
        directory = juce::File(vt->getProperty(param_directory));
        stereoMode = (Parameters::enumStereo) int(vt->getProperty(param_stereoMode));

        juce::String extension = file.getFileExtension();
        if (extension == ".wav" || extension == ".aiff") {
            convolution.loadImpulseResponse(file,
                juce::dsp::Convolution::Stereo::yes,
                juce::dsp::Convolution::Trim::yes,
                0);
        }
        /*
        */
    }

    // Update DSP with atomic values
    void updateParameters() {
        mix.setWetMixProportion(valueMix->load());
        gain.setGainDecibels(valueGain->load());
        filterLowCut.filterFrequency = valueLowCut->load();
        filterHighCut.filterFrequency = valueHighCut->load();
        pan.setWetMixProportion(valuePan->load());
        // valueDelay->load();
        // valueBypass->load();
        // valueInvert->load();
    }

    // TODO: Parameter object would trigger onChange callback
    void setFile(juce::File value) {
        DBG("setFile()" + suffix);
        if (!value.existsAsFile()) { return; }
        juce::String extension = file.getFileExtension();
        if (extension != ".wav" && extension != ".aiff") { return; }
        
        file = value;
        directory = value.getParentDirectory().getFullPathName();
        variableTree->setProperty(param_file, file.getFullPathName(), nullptr);
        variableTree->setProperty(param_directory, file.getParentDirectory().getFullPathName(), nullptr);
        
        convolution.loadImpulseResponse(file,
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::yes,
            0);
    }

    void process(
        juce::dsp::ProcessContextReplacing<float> inputContext, 
        juce::AudioBuffer<float> outputBuffer )
    {
        buffer.makeCopyOf(outputBuffer, true); // Copy dry buffer
        juce::dsp::AudioBlock<float> audioBlock = { buffer };
        juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
        
        if (convolution.getCurrentIRSize() > 0) {
            convolution.process(context);
        }

        mix.pushDrySamples(inputContext.getOutputBlock());
        mix.mixWetSamples(audioBlock);
        gain.process(context);
        /*
        filterLowCut.process(buffer);
        filterHighCut.process(buffer);
        // TODO: Pan
        */
    }

    void process_combine(
        juce::dsp::ProcessContextReplacing<float> contextLeft,
        juce::dsp::ProcessContextReplacing<float> contextRight,
        juce::AudioBuffer<float> bufferLeft,
        juce::AudioBuffer<float> bufferRight )
    {
        buffer.makeCopyOf(bufferRight, true); // Copy dry buffer
        juce::dsp::AudioBlock<float> audioBlock = { buffer };
        juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(audioBlock);
        mix.pushDrySamples(contextLeft.getOutputBlock());
        mix.mixWetSamples(context.getOutputBlock());
        gain.process(context);

        /*
        // TODO: Pan
        filterLowCut.process(buffer);
        filterHighCut.process(buffer);
        */
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorGroup)
};