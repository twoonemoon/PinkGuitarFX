#include "PluginProcessor.h"
#include "PluginEditor.h"

PinkGuitarFXAudioProcessor::PinkGuitarFXAudioProcessor()
    : AudioProcessor (BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true).withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters", createParameterLayout())
{}

PinkGuitarFXAudioProcessor::~PinkGuitarFXAudioProcessor() {}

void PinkGuitarFXAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32)samplesPerBlock, 2 };
    oversampling.initProcessing(samplesPerBlock);
    masterLevel.prepare(spec);
    
    auto initF = [&](juce::dsp::StateVariableTPTFilter<float>& f, juce::dsp::StateVariableTPTFilter<float>::Type t, float freq) {
        f.prepare(spec); f.setType(t); f.setCutoffFrequency(freq);
    };
    initF(lowFilterL, juce::dsp::StateVariableTPTFilter<float>::Type::lowpass, 200.0f);
    initF(lowFilterR, juce::dsp::StateVariableTPTFilter<float>::Type::lowpass, 200.0f);
    initF(highFilterL, juce::dsp::StateVariableTPTFilter<float>::Type::highpass, 3000.0f);
    initF(highFilterR, juce::dsp::StateVariableTPTFilter<float>::Type::highpass, 3000.0f);

    convolutionL.prepare(spec); convolutionR.prepare(spec);
}

void PinkGuitarFXAudioProcessor::loadImpulseResponse(int cabIndex) {
    if (cabIndex == currentCabIndex) return;
    currentCabIndex = cabIndex;
    const void* data = nullptr; size_t size = 0;
    if (cabIndex == 0) { data = BinaryData::lol_wav; size = (size_t)BinaryData::lol_wavSize; }
    else if (cabIndex == 1) { data = BinaryData::kek_wav; size = (size_t)BinaryData::kek_wavSize; }
    else if (cabIndex == 2) { data = BinaryData::mek_wav; size = (size_t)BinaryData::mek_wavSize; }

    if (data && cabIndex != 3) {
        convolutionL.loadImpulseResponse(data, size, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
        convolutionR.loadImpulseResponse(data, size, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::yes, 0);
    }
}

void PinkGuitarFXAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    float drive = apvts.getRawParameterValue("gain")->load();
    float wide = apvts.getRawParameterValue("wide")->load();
    float gate = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("gate")->load());
    int cabMode = (int)apvts.getRawParameterValue("cab")->load();

    loadImpulseResponse(cabMode);

    for (int c = 0; c < buffer.getNumChannels(); ++c) {
        for (int s = 0; s < buffer.getNumSamples(); ++s) {
            if (std::abs(buffer.getSample(c, s)) < gate) buffer.setSample(c, s, 0);
        }
    }

    juce::dsp::AudioBlock<float> block (buffer);
    auto osBlock = oversampling.processSamplesUp (block);
    
    for (int i = 0; i < (int)osBlock.getNumSamples(); ++i) {
        float input = osBlock.getChannelPointer(0)[i];
        float x = input * (1.0f + drive * 40.0f);
        float dist = (x > 0) ? 1.0f - std::exp(-x) : -1.0f + std::exp(x * 1.3f);
        
        float l = lowFilterL.processSample(0, dist) * std::pow(10.0f, apvts.getRawParameterValue("low")->load()/20.0f);
        float h = highFilterL.processSample(0, dist) * std::pow(10.0f, apvts.getRawParameterValue("high")->load()/20.0f);
        dist = l + h + (dist * 0.4f);

        osBlock.getChannelPointer(0)[i] = dist + (std::sin(dist) * wide);
        osBlock.getChannelPointer(1)[i] = dist - (std::sin(dist) * wide);
    }
    oversampling.processSamplesDown (block);

    if (cabMode != 3) {
        auto leftBlock = block.getSingleChannelBlock(0);
        auto rightBlock = block.getSingleChannelBlock(1);
        convolutionL.process(juce::dsp::ProcessContextReplacing<float>(leftBlock));
        convolutionR.process(juce::dsp::ProcessContextReplacing<float>(rightBlock));
    }

    rmsLevelLeft.store(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
    masterLevel.setGainDecibels(apvts.getRawParameterValue("master")->load());
    masterLevel.process(juce::dsp::ProcessContextReplacing<float>(block));
}

void PinkGuitarFXAudioProcessor::releaseResources() {}

juce::AudioProcessorValueTreeState::ParameterLayout PinkGuitarFXAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout l;
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gain", 1}, "DRIVE", 0.0f, 1.0f, 0.7f));
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"gate", 1}, "GATE", -80.0f, -20.0f, -60.0f));
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"wide", 1}, "WIDE", 0.0f, 1.0f, 0.2f));
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"low", 1}, "BASS", -12.0f, 12.0f, 0.0f));
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"high", 1}, "TREB", -12.0f, 12.0f, 0.0f));
    l.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"master", 1}, "MASTER", -60.0f, 12.0f, 0.0f));
    l.add(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID{"cab", 1}, "CAB", juce::StringArray{"Lol IR", "Kek IR", "Mek IR", "OFF"}, 0));
    return l;
}

juce::AudioProcessorEditor* PinkGuitarFXAudioProcessor::createEditor() { return new PinkGuitarFXAudioProcessorEditor (*this); }
void PinkGuitarFXAudioProcessor::getStateInformation (juce::MemoryBlock& d) { copyXmlToBinary (*apvts.copyState().createXml(), d); }
void PinkGuitarFXAudioProcessor::setStateInformation (const void* d, int s) { auto xml = getXmlFromBinary(d,s); if(xml) apvts.replaceState(juce::ValueTree::fromXml(*xml)); }
