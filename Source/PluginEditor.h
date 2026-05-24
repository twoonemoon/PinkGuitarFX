#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "TunerComponent.h"

class PinkGuitarFXAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    PinkGuitarFXAudioProcessorEditor (PinkGuitarFXAudioProcessor&);
    ~PinkGuitarFXAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    PinkGuitarFXAudioProcessor& audioProcessor;
    PinkKnobLook pinkLook;
    TunerComponent tuner;

    juce::OwnedArray<juce::Slider> knobs;
    juce::OwnedArray<juce::Label> labels;
    juce::Slider masterSlider;
    juce::ComboBox cabBox;

    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> sAttaches;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkGuitarFXAudioProcessorEditor)
};
