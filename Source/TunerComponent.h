#pragma once
#include <JuceHeader.h>

class PinkKnobLook : public juce::LookAndFeel_V4 {
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           float rotaryStartAngle, float rotaryEndAngle, juce::Slider& s) override {
        auto radius = (float)juce::jmin (width / 2, height / 2) - 8.0f;
        auto centreX = (float)x + (float)width  * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colours::black.withAlpha(0.5f));
        g.fillEllipse(centreX - radius + 2, centreY - radius + 4, radius * 2, radius * 2);

        juce::ColourGradient grad(juce::Colour(0xFF252525), centreX, centreY - radius, juce::Colour(0xFF0F0F0F), centreX, centreY + radius, false);
        g.setGradientFill(grad);
        g.fillEllipse(centreX - radius, centreY - radius, radius * 2, radius * 2);

        g.setColour(juce::Colours::hotpink);
        juce::Path p;
        p.addRoundedRectangle(-2.0f, -radius + 2.0f, 4.0f, radius * 0.5f, 2.0f);
        g.fillPath(p, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }
};

class TunerComponent : public juce::Component {
public:
    void updatePitch (float freq) {
        if (freq > 30.0f && freq < 1500.0f) {
            const char* notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
            float rawNote = 12.0f * std::log2(freq / 440.0f) + 69.0f;
            int m = std::round(rawNote);
            noteName = notes[m % 12];
        } else { noteName = "-"; }
        repaint();
    }

    void paint (juce::Graphics& g) override {
        auto b = getLocalBounds().toFloat();
        g.setColour(juce::Colour(0xFF111111));
        g.fillRoundedRectangle(b, 10.0f);
        g.setColour(juce::Colours::hotpink.withAlpha(0.3f));
        g.drawRoundedRectangle(b, 10.0f, 2.0f);
        
        g.setColour(juce::Colours::hotpink);
        g.setFont(juce::FontOptions(32.0f).withStyle("Bold"));
        g.drawText(noteName, b, juce::Justification::centred);
    }
private:
    juce::String noteName = "-";
};
