#pragma once
#include <JuceHeader.h>

class PinkGuitarFXAudioProcessor : public juce::AudioProcessor {
public:
    PinkGuitarFXAudioProcessor();
    ~PinkGuitarFXAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "safoniy2012"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int index) override {}
    const juce::String getProgramName (int index) override { return {}; }
    void changeProgramName (int index, const juce::String& newName) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    std::atomic<float> rmsLevelLeft { 0.0f };

    void loadImpulseResponse(int cabIndex);

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::dsp::Oversampling<float> oversampling { 2, 2, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };
    juce::dsp::Convolution convolutionL, convolutionR;
    
    juce::dsp::StateVariableTPTFilter<float> lowFilterL, lowFilterR;
    juce::dsp::StateVariableTPTFilter<float> highFilterL, highFilterR;
    
    juce::dsp::Gain<float> masterLevel;
    int currentCabIndex = -1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinkGuitarFXAudioProcessor)
};
