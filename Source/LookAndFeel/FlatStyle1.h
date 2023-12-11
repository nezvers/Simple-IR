/*
  ==============================================================================

    FlatStyle1.h
    Created: 11 Dec 2023 6:57:03pm
    Author:  NeZvers

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct FlatStyle1 : public LookAndFeel_V4
{
    /*juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider){

        juce::Slider::SliderLayout layout = juce::LookAndFeel_V4::getSliderLayout(slider);

        auto localBounds = slider.getLocalBounds();

        auto textBoxWidth = layout.textBoxBounds.getWidth();
        auto textBoxHeight = layout.textBoxBounds.getHeight();
        layout.textBoxBounds.setX((localBounds.getWidth() - textBoxWidth) / 2);
        layout.textBoxBounds.setY((localBounds.getHeight() - textBoxHeight) / 2);

        return layout;
    }*/

    void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override
    {
        const float margin = 2.0f;
        const float radius = (float)jmin(width / 2, height / 2) - margin;
        const float centreX = (float)x + (float)width * 0.5f;
        const float centreY = (float)y + (float)height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

        
        {
            Path outlineArc;
            //outlineArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.0);
            outlineArc.addArc(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, true);
            float lineThickness = jmin(15.0f, (float)jmin(width, height) * 0.45f) * 0.2f;
            

            if (slider.isEnabled()) {
                g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(isMouseOver ? 1.0f : 0.7f));
            }
            else {
                g.setColour(slider.findColour(Slider::rotarySliderFillColourId).withAlpha(0.5f));
            }
            g.strokePath(outlineArc, PathStrokeType(lineThickness));
        }

        {
            Path outlineArc;
            outlineArc.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
            float lineThickness = jmin(15.0f, (float)jmin(width, height) * 0.45f) * 0.2f;
            float gray = 0.98f;
            
            if (slider.isEnabled()) {
                g.setColour(juce::Colour::fromFloatRGBA(gray, gray, gray, 1.0).withAlpha(isMouseOver ? 1.0f : 0.7f));
            }
            else {
                g.setColour(juce::Colour::fromFloatRGBA(gray, gray, gray, 1.0).withAlpha(0.5f));
            }
            g.strokePath(outlineArc, PathStrokeType(lineThickness));

            float x1 = centreX + std::cos(angle - juce::MathConstants<float>::halfPi) * radius * 0.7f;
            float y1 = centreY + std::sin(angle - juce::MathConstants<float>::halfPi) * radius * 0.7f;
            float x2 = centreX + std::cos(angle - juce::MathConstants<float>::halfPi) * radius * 0.9f;
            float y2 = centreY + std::sin(angle - juce::MathConstants<float>::halfPi) * radius * 0.9f;
            g.drawLine(x1, y1, x2, y2, lineThickness * 0.5f);
        }


    }
};