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
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider){

        juce::Slider::SliderLayout layout = juce::LookAndFeel_V4::getSliderLayout(slider);

        juce::Rectangle<int> localBounds = slider.getLocalBounds();
        layout.sliderBounds = localBounds;

        int centreX = localBounds.getWidth() * 0.5f;
        int centreY = localBounds.getHeight() * 0.5f;

        int textBoxWidth = centreX * 1.4f;
        int textBoxHeight = centreY * 0.4f;
        layout.textBoxBounds.setX(centreX - textBoxWidth * 0.5f);
        layout.textBoxBounds.setY(centreY - textBoxHeight);
        layout.textBoxBounds.setWidth(textBoxWidth);
        layout.textBoxBounds.setHeight(textBoxHeight);

        return layout;
    }


    Label* createSliderTextBox(Slider& slider) override {
        juce::Label* l = LookAndFeel_V2::createSliderTextBox(slider);
        l->setInterceptsMouseClicks(false, false);
        
        const float gray = 0.98f;
        l->setColour(Label::textColourId, juce::Colour::fromFloatRGBA(gray, gray, gray, 1.0));
        l->setBorderSize(juce::BorderSize<int>(0));
        
        l->setColour(Label::outlineColourId, juce::Colour::fromRGBA(0.f, 0.f, 0.f, 0.f));
        
        Font font = l->getFont();
        float fontHeight = font.getHeight() * 0.9f;
        font.setHeight(fontHeight);
        l->setFont(font);

        return l;
    };

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
        const bool isMouseOver = slider.isMouseOverOrDragging() && slider.isEnabled();

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
            Path valueArc;
            valueArc.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
            float lineThickness = jmin(15.0f, (float)jmin(width, height) * 0.45f) * 0.2f;
            juce::Point endPoint = valueArc.getCurrentPosition();
            float x = endPoint.getX() + (centreX - endPoint.getX()) * 0.1f;
            float y = endPoint.getY() + (centreY - endPoint.getY()) * 0.1f;
            juce::Line<float> line = juce::Line<float>(endPoint, juce::Point(x, y));
            valueArc.addLineSegment(line, lineThickness * 0.2f);
            
            float gray = 0.98f;
            if (slider.isEnabled()) {
                g.setColour(juce::Colour::fromFloatRGBA(gray, gray, gray, 1.0).withAlpha(isMouseOver ? 1.0f : 0.7f));
            }
            else {
                g.setColour(juce::Colour::fromFloatRGBA(gray, gray, gray, 1.0).withAlpha(0.5f));
            }
            g.strokePath(valueArc, PathStrokeType(lineThickness));

            /*float x1 = centreX + std::cos(angle - juce::MathConstants<float>::halfPi) * radius * 0.85f;
            float y1 = centreY + std::sin(angle - juce::MathConstants<float>::halfPi) * radius * 0.85f;
            float x2 = centreX + std::cos(angle - juce::MathConstants<float>::halfPi) * radius * 1.0f + lineThickness * 0.5;
            float y2 = centreY + std::sin(angle - juce::MathConstants<float>::halfPi) * radius * 1.0f + lineThickness * 0.5;
            g.drawLine(x1, y1, x2, y2, lineThickness);*/
        }


    }
};