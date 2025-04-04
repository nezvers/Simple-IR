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
#include "../Parameters/Parameters.h"
#include <vector>
#include "../LookAndFeel/FlatStyle1.h"
#include "../Components/SliderWithLabel.h"
#include "../Components/ButtonSvg.h"

class ProcessorGroup {
public:
    ProcessorGroup(juce::String idSuffix, bool _is_output) {
        suffix = idSuffix;
        DBG("_ProcessorGroup() " + suffix);
        param_file += idSuffix;
        param_directory += idSuffix;
        id_stereoMode += idSuffix;
        id_mix += idSuffix;
        id_gain += idSuffix;
        id_lowCut += idSuffix;
        id_highCut += idSuffix;
        id_pan += idSuffix;
        id_delay += idSuffix;
        id_bypass += idSuffix;
        id_invert += idSuffix;

        is_output = _is_output;
        if (is_output) {
            name_gain = "Output";
            name_pan = "Reverb";
            name_bypass = "Bypass";
            buttonBypass.setSvg(BinaryData::enable_on_svg);
            buttonInvert.setSvg(BinaryData::phase_classic_svg);
        }
        else {
            name_bypass = "S";
            buttonBypass.setSvg(BinaryData::solo_svg);
            buttonInvert.setSvg(BinaryData::phase_classic_svg);
        }
    };
    ~ProcessorGroup() {
        sliderMix.setLookAndFeel(nullptr);
        sliderGain.setLookAndFeel(nullptr);
        sliderLowCut.setLookAndFeel(nullptr);
        sliderHighCut.setLookAndFeel(nullptr);
        sliderPan.setLookAndFeel(nullptr);
        sliderDelay.setLookAndFeel(nullptr);
        buttonBypass.setLookAndFeel(nullptr);
        buttonInvert.setLookAndFeel(nullptr);
        comboStereoMode.setLookAndFeel(nullptr);
    };

#pragma region Parameter IDs
    juce::String suffix;
    juce::String param_file = "file";
    juce::String param_directory = "directory";
    juce::String id_stereoMode = "stereo";
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
        juce::String name_gain = "Gain";
    const juce::String name_lowCut = "Low Cut";
    const juce::String name_highCut = "High Cut";
        juce::String name_pan = "Pan";
    const juce::String name_delay = "Delay";
        juce::String name_bypass = "S";
    const juce::String name_invert = "P";
    const juce::String name_stereoMode = "Stereo Mode";
#pragma endregion

#pragma region value links
    std::atomic<float>* valueMix = nullptr;
    std::atomic<float>* valueGain = nullptr;
    std::atomic<float>* valueLowCut= nullptr;
    std::atomic<float>* valueHighCut = nullptr;
    std::atomic<float>* valuePan = nullptr;
    std::atomic<float>* valueReverb = nullptr;
    std::atomic<float>* valueDelay = nullptr;
    std::atomic<float>* valueBypass = nullptr;
    std::atomic<float>* valueInvert = nullptr;
    std::atomic<float>* valueStereoMode = nullptr;
#pragma endregion

#pragma region Components
    SliderWithLabel sliderMix;//
    SliderWithLabel sliderGain;
    SliderWithLabel sliderLowCut;
    SliderWithLabel sliderHighCut;
    SliderWithLabel sliderPan;
    SliderWithLabel sliderDelay;
    ButtonSvg buttonBypass;
    ButtonSvg buttonInvert;
    juce::ComboBox comboStereoMode;
#pragma endregion

#pragma region Attachments
    using nSliderAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>;
    using nButtonAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>;
    using nComboBoxAttachment = std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>;

    nSliderAttachment attachmentMix;
    nSliderAttachment attachmentGain;
    nSliderAttachment attachmentLowCut;
    nSliderAttachment attachmentHighCut;
    nSliderAttachment attachmentPan;
    nSliderAttachment attachmentDelay;
    nButtonAttachment attachmentBypass;
    nButtonAttachment attachmentInvert;
    nComboBoxAttachment attachmentStereoMode;
#pragma endregion

#pragma region DSP
    juce::dsp::DryWetMixer<float> mix;
    juce::dsp::DryWetMixer<float> mixReverb;
    juce::dsp::Panner<float> pan;
    juce::dsp::Gain<float> gain;
    juce::dsp::Convolution convolution;
    //LowHighCutFilters filterLowCut;
    //LowHighCutFilters filterHighCut;
    using DublicatedFilter = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    DublicatedFilter filterLowCut { juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, 20.0f, 1.0f) };
    DublicatedFilter filterHighCut { juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 1.0f) };

    juce::AudioBuffer<float> buffer;
    juce::File file = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    juce::File directory = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    Parameters::enumStereo stereoMode = Parameters::enumStereo::DUAL_MONO;
    bool is_output = false;
    ProcessorGroup* other = nullptr;
    FlatStyle1 lookAndFeel;
    juce::dsp::ProcessSpec spec;
    bool isDualMono = false;
    bool isStereo = true;
    bool isMono = false;
    ProcessorGroup* procLeft; // only for output
    ProcessorGroup* procRight; // only for output
#pragma endregion

    // Initializes DSP
    void setSpec(juce::dsp::ProcessSpec& _spec) {
        spec = _spec;
        gain.prepare(spec);
        mix.prepare(spec);
        if (is_output) {
            mixReverb.prepare(spec);
        }
        else {
            pan.prepare(spec);
        }
        convolution.prepare(spec);
        gain.setRampDurationSeconds(0.01);

        filterLowCut.prepare(spec);
        filterHighCut.prepare(spec);
        
        filterLowCut.reset();
        filterHighCut.reset();
    }

    void init(juce::AudioProcessorValueTreeState* vts) {
        DBG("init() " + suffix);

        initLinks(vts);
        initSliders();
        initButtons();
        initComboBoxes();

        // Connect attachments to GUI components
        attachmentMix = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_mix, sliderMix);
        attachmentGain = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_gain, sliderGain);
        attachmentLowCut = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_lowCut, sliderLowCut);
        attachmentHighCut = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_highCut, sliderHighCut);
        attachmentPan = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_pan, sliderPan);
        if (is_output) {
            auto param = vts->getParameter(id_stereoMode);
            DBG(juce::String("Parameter: ") + juce::String(""));
            attachmentStereoMode = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(*vts, id_stereoMode, comboStereoMode);
        }
        else {
            attachmentDelay = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(*vts, id_delay, sliderDelay);
            attachmentInvert = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*vts, id_invert, buttonInvert);
        }
        attachmentBypass = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(*vts, id_bypass, buttonBypass);
    }
    // Links atomic valiables
    void initLinks(juce::AudioProcessorValueTreeState* vts) {
        DBG("initLinks() " + suffix);
        valueMix = vts->getRawParameterValue(id_mix);
        valueGain = vts->getRawParameterValue(id_gain);
        valueLowCut = vts->getRawParameterValue(id_lowCut);
        valueHighCut = vts->getRawParameterValue(id_highCut);
        if (is_output) {
            valueStereoMode = vts->getRawParameterValue(id_stereoMode);
            valueReverb = vts->getRawParameterValue(id_pan);
        }
        else {
            valueDelay = vts->getRawParameterValue(id_delay);
            valueInvert = vts->getRawParameterValue(id_invert);
            valuePan = vts->getRawParameterValue(id_pan);
        }
        valueBypass = vts->getRawParameterValue(id_bypass);
    }
    
    void initSliders()
    {
        DBG("Init Sliders ");
        SliderWithLabel* sliderList[] = {
            &sliderMix,
            &sliderGain,
            &sliderLowCut,
            &sliderHighCut,
            &sliderPan,
            &sliderDelay
        };
        juce::String nameList[] = {
            name_mix,
            name_gain,
            name_lowCut,
            name_highCut,
            name_pan,
            name_delay
        };
        float minList[] = {
            0.0f,
            Parameters::gainMin,
            Parameters::freqMin,
            Parameters::freqMin,
            -100.0f,
            Parameters::delayMin
        };
        float maxList[] = {
            1.0f,
            Parameters::gainMax,
            Parameters::freqMax,
            Parameters::freqMax,
            100.0f,
            Parameters::delayMax
        };
        const float skewList[] = {
            1.0f,
            2.0f,
            10.0f,
            10.0f,
            1.0f,
            0.5f
        };
        double intervalList[] = {
            0.01,
            0.01,
            1,
            1,
            0.01,
            1
        };
        float returnList[] = {
            1.0f,
            0.0f,
            20.0f,
            20000.0f,
            0.0f,
            0.0f
        };
        const juce::String suffixList[] = {
            "",
            "dB",
            "Hz",
            "Hz",
            "",
            ""
        };

        if (is_output) {
            minList[4] = 0.0f;
            maxList[4] = 1.0f;
            returnList[4] = 0.0f;
        }
        enum { MIX, GAIN, LOWCUT, HIGHCUT, PAN, DELAY, COUNT };
        for (int i = 0; i < COUNT; i++) {
            SliderWithLabel& slider = *sliderList[i];
            slider.setLookAndFeel(&lookAndFeel);
            slider.setName(nameList[i]);
            slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 72, 32);
            slider.setSkewFactor(skewList[i], false);
            slider.setRange(minList[i], maxList[i], intervalList[i]);
            slider.setNumDecimalPlacesToDisplay(0);
            slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
            slider.setDoubleClickReturnValue(true, returnList[i], juce::ModifierKeys::ctrlModifier);
            slider.label.setText(nameList[i], juce::NotificationType::dontSendNotification);
            //slider.setTextValueSuffix(suffixList[i]);
        }
    }

    void initButtons()
    {
        DBG("Init Buttons " + suffix);
        //buttonBypass.setButtonText(name_bypass);
        //buttonInvert.setButtonText(name_invert);

        buttonBypass.setToggleable(true);
        buttonInvert.setToggleable(true);

        buttonBypass.setClickingTogglesState(true);
        buttonInvert.setClickingTogglesState(true);

        buttonBypass.setLookAndFeel(&lookAndFeel);
        buttonInvert.setLookAndFeel(&lookAndFeel);
    }

    void initComboBoxes() {
        if (!is_output) { return; }
        DBG("Init Combo boxes");
        comboStereoMode.addItem("Dual Mono", 1);
        comboStereoMode.addItem("Stereo", 2);
        comboStereoMode.addItem("Mono", 3);
        comboStereoMode.setLookAndFeel(&lookAndFeel);
    }

    std::vector<juce::Component*> getComponentList() {
        std::vector < juce::Component*> list = {
            &sliderMix,
            &sliderGain,
            &sliderLowCut,
            &sliderHighCut,
            &sliderPan,
            &sliderDelay,
            &buttonBypass,
            &buttonInvert,
            &comboStereoMode
        };
        // remove components that are not drawn
        if (is_output) {
            list.erase(list.begin() + 5);
        }
        else {
            list.erase(list.begin() + 8);
        }
        return list;
    }

    // Generates ValueTreeState parameters
    void setParameterLayout(std::vector <std::unique_ptr<juce::RangedAudioParameter>>& params) {
        DBG("setParameterLayout() " + suffix);
        //tree.createAndAddParameter("id", "name", ...)
        
        NormalisableRange<float> gainRange {Parameters::gainMin, Parameters::gainMax, 0.01f, 2.0f};
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            id_gain,
            name_gain,
            gainRange,
            0.0f,
            name_gain,
            AudioProcessorParameter::genericParameter,
            Parameters::valToStr::db(),
            Parameters::strToVal::db()
        ));
        
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            id_lowCut, 
            name_lowCut, 
            Parameters::range::withCentre(20.0f, 20000.0f, 1000.0f),
            Parameters::freqMin,
            name_lowCut,
            AudioProcessorParameter::genericParameter,
            Parameters::valToStr::hz(),
            Parameters::strToVal::hz()
        ));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            id_highCut, 
            name_highCut, 
            Parameters::range::withCentre(20.0f, 20000.0f, 1000.0f),
            Parameters::freqMax,
            name_highCut,
            AudioProcessorParameter::genericParameter,
            Parameters::valToStr::hz(),
            Parameters::strToVal::hz()
        ));
        
        if (is_output)
        {
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                id_mix, name_mix, 
                Parameters::range::lin(0.0f, 1.0f),
                0.0f,
                name_mix,
                AudioProcessorParameter::genericParameter,
                Parameters::valToStr::mix(),
                Parameters::strToVal::mix()
            ));
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                id_pan, name_pan, 
                Parameters::range::lin(0.0f, 1.0f),
                0.0f,
                name_pan,
                AudioProcessorParameter::genericParameter,
                Parameters::valToStr::mix(),
                Parameters::strToVal::mix()
            ));
        }
        else 
        {
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                id_pan, 
                name_pan, 
                Parameters::range::lin(0.0f, 1.0f),
                0.5f,
                name_pan,
                AudioProcessorParameter::genericParameter,
                Parameters::valToStr::pan(),
                Parameters::strToVal::pan()
            ));
            params.push_back(std::make_unique<juce::AudioParameterFloat>(
                id_mix, name_mix, 
                Parameters::range::lin(0.0f, 1.0f),
                0.5f,
                name_mix,
                AudioProcessorParameter::genericParameter,
                Parameters::valToStr::mix(),
                Parameters::strToVal::mix()
            ));
            params.push_back(std::make_unique<juce::AudioParameterFloat>(id_delay, name_delay, 0.0f, 512.0f, 0.0f));
        }

        params.push_back(std::make_unique<juce::AudioParameterChoice>(id_stereoMode, name_stereoMode, StringArray("Dual Mono", "Stereo", "Mono"), 0));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_invert, name_invert, 0.0f, 1.0f, 0.0f));
        params.push_back(std::make_unique<juce::AudioParameterFloat>(id_bypass, name_bypass, 0.0f, 1.0f, 0.0f));
    }

    // LOAD
    void setStateInformation(juce::ValueTree* vt) {
        DBG("setStateInformation()" + suffix);        

        juce::File fileLoad = juce::File(vt->getProperty(param_file));
        if (!fileLoad.existsAsFile()) {
            return;
        }
        file = fileLoad;
        directory = file.getParentDirectory().getFullPathName();

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
        

        *filterLowCut.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass((int)spec.sampleRate, valueLowCut->load(), 1.0f);
        *filterHighCut.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass((int)spec.sampleRate, valueHighCut->load(), 1.0f);
        
        if (is_output) {
            gain.setGainDecibels(valueGain->load());
            mixReverb.setWetMixProportion(valueReverb->load());

            stereoMode = Parameters::enumStereo(int(valueStereoMode->load()));
            int value = (int)valueStereoMode->load();
            isDualMono = value == 0;
            isStereo = value == 1;
            isMono = value == 2;

            if (isStereo != procLeft->isStereo) {
                juce::MessageManagerLock lock;
                procLeft->isStereo = isStereo;
                procLeft->sliderPan.setEnabled(isStereo);
            }
            if (isStereo != procRight->isStereo) {
                juce::MessageManagerLock lock;
                procRight->isStereo = isStereo;
                procRight->sliderPan.setEnabled(isStereo);
            }
            mixReverb.setWetMixProportion(valueReverb->load());
        }
        else {
            if (int(valueInvert->load()) == 0) {
                gain.setGainDecibels(valueGain->load());
            }
            else {
                gain.setGainDecibels(valueGain->load() * -1.0f);
            }
            if (isStereo) {
                pan.setPan(valuePan->load());
            }
        }
    }

    // TODO: Parameter object would trigger onChange callback
    void setFile(juce::File value, juce::ValueTree* vt) {
        DBG("setFile()" + suffix);
        if (!value.existsAsFile()) { return; }

        juce::String extension = value.getFileExtension();
        if (extension != ".wav" && extension != ".aiff") { return; }
        DBG(value.getFileName());

        file = value;
        directory = value.getParentDirectory().getFullPathName();
        juce::String full_path = file.getFullPathName();
        juce::Identifier param_file_identifier = juce::Identifier(param_file);
        vt->setProperty(param_file_identifier, full_path, nullptr);
        vt->setProperty(param_directory, directory.getFullPathName(), nullptr);
        
        convolution.loadImpulseResponse(file,
            juce::dsp::Convolution::Stereo::yes,
            juce::dsp::Convolution::Trim::yes,
            0);
    }

    void setLinkButtons(ProcessorGroup& _other) {
        other = &_other;
        buttonBypass.onClick = [this]() {
            if (!buttonBypass.getToggleState()) {
                return;
            }
            
            if (other->buttonBypass.getToggleState()) {
                other->buttonBypass.setToggleState(false, juce::sendNotification);
            }
        };
        other->buttonBypass.onClick = [this]() {
            if (!other->buttonBypass.getToggleState()) {
                return;
            }

            if (buttonBypass.getToggleState()) {
                buttonBypass.setToggleState(false, juce::sendNotification);
            }
        };
    }

    void process(
        juce::AudioBuffer<float> outputBuffer) {
        updateParameters();

        buffer.makeCopyOf(outputBuffer, true); // Copy dry buffer
        juce::dsp::AudioBlock<float> audioBlock = { buffer };
        juce::dsp::ProcessContextReplacing<float> context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

        if (convolution.getCurrentIRSize() > 0) {
            convolution.process(context);
        }

        mix.pushDrySamples(outputBuffer);
        mix.mixWetSamples(audioBlock);
        gain.process(context);
        
        bool isInverted = int(valueInvert->load()) == 1;
        
        if (isInverted) {
            buffer.applyGain(-1.0f);
        }

        filterLowCut.process(context);
        filterHighCut.process(context);

        if (isStereo) {
            pan.process(context);
        }
    }

    void process_output(juce::AudioBuffer<float> incommingBuffer) {
        updateParameters();
        const bool DISABLED = int(valueBypass->load()) == 1;
        if (DISABLED) { return; }

        if (isMono && spec.numChannels > 1) {
            // Left channel into right channel
            incommingBuffer.copyFrom(1, 0, incommingBuffer, 0, 0, incommingBuffer.getNumSamples());
        }
        bool soloLeft = int(procLeft->valueBypass->load()) == 1;
        bool soloRight = int(procRight->valueBypass->load()) == 1;
        
        
        if (soloRight || soloLeft) {
            if (soloRight) {
                procRight->process(incommingBuffer);
                incommingBuffer.makeCopyOf(procRight->buffer, true); // Copy for dry buffer
            }
            else {
                procLeft->process(incommingBuffer);
                incommingBuffer.makeCopyOf(procLeft->buffer, true); // Copy for dry buffer
            }
        }
        else {
            procRight->process(incommingBuffer);
            procLeft->process(incommingBuffer);

            mix.pushDrySamples(procLeft->buffer);
            mix.mixWetSamples(procRight->buffer); // Right buffer holds IR mix
            
            incommingBuffer.makeCopyOf(procRight->buffer, true); // Copy for dry buffer
        }

        
        mixReverb.pushDrySamples(incommingBuffer); // It's Reverb mix for output (WIP)
        
        juce::dsp::AudioBlock<float> audioBlockOut = { incommingBuffer };
        juce::dsp::ProcessContextReplacing<float> contextOut = juce::dsp::ProcessContextReplacing<float>(audioBlockOut);

        if (convolution.getCurrentIRSize() > 0) {
            convolution.process(contextOut);
        }

        mixReverb.mixWetSamples(incommingBuffer);

        gain.process(contextOut);
        filterLowCut.process(contextOut);
        filterHighCut.process(contextOut);
    }

    void _process(
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

        switch (stereoMode) {
            case Parameters::DUAL_MONO:
                break;
            case Parameters::STEREO:
                break;
            case Parameters::MONO:
                break;
        }

        /*
        // TODO: Pan
        filterLowCut.process(buffer);
        filterHighCut.process(buffer);
        */
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorGroup)
};