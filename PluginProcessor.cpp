#include "PluginProcessor.h"
#include "PluginEditor.h"

GraphicEQ15BandAudioProcessor::GraphicEQ15BandAudioProcessor()
    : AudioProcessor(BusesProperties()
                    .withInput("Input", juce::AudioChannelSet::stereo(), true)
                    .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, juce::Identifier("GraphicEQ15Band"), createParameterLayout())
{
    for (int i = 0; i < numBands; ++i)
    {
        gains[i] = 0.0f;
    }
}

GraphicEQ15BandAudioProcessor::~GraphicEQ15BandAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout GraphicEQ15BandAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    for (int i = 0; i < numBands; ++i)
    {
        params.push_back(std::make_unique<juce::AudioParameterFloat>(
            "GAIN_" + juce::String(i),
            "Band " + juce::String(frequencies[i]) + " Hz",
            juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
            0.0f));
    }
    return { params.begin(), params.end() };
}

void GraphicEQ15BandAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (int band = 0; band < numBands; ++band)
    {
        auto* filterL = &filters[band * 2];
        auto* filterR = &filters[band * 2 + 1];
        
        float q = 1.41f; // Fixed Q for simplicity (approx 1/3 octave)
        // Initialize with flat response if gain is 0 dB
        if (std::abs(gains[band]) < 0.01f)
        {
            filterL->reset();
            filterR->reset();
        }
        else
        {
            auto coeffs = juce::IIRCoefficients::makePeakFilter(sampleRate, frequencies[band], q, juce::Decibels::decibelsToGain(gains[band]));
            filterL->setCoefficients(coeffs);
            filterR->setCoefficients(coeffs);
        }
    }
}

void GraphicEQ15BandAudioProcessor::releaseResources()
{
    for (auto& filter : filters)
        filter.reset();
}

void GraphicEQ15BandAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto numSamples = buffer.getNumSamples();
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);

    // Check if input is silent to skip processing
    bool isSilent = true;
    for (int i = 0; i < numSamples; ++i)
    {
        if (std::abs(leftChannel[i]) > 1e-6f || std::abs(rightChannel[i]) > 1e-6f)
        {
            isSilent = false;
            break;
        }
    }

    if (isSilent)
    {
        buffer.clear(); // Clear output if input is silent
        return;
    }

    // Update filter coefficients if gains have changed
    for (int band = 0; band < numBands; ++band)
    {
        float newGain = *parameters.getRawParameterValue("GAIN_" + juce::String(band));
        if (std::abs(newGain - gains[band]) > 0.01f)
        {
            gains[band] = newGain;
            float q = 1.41f;
            auto* filterL = &filters[band * 2];
            auto* filterR = &filters[band * 2 + 1];
            
            if (std::abs(newGain) < 0.01f)
            {
                filterL->reset();
                filterR->reset();
            }
            else
            {
                auto coeffs = juce::IIRCoefficients::makePeakFilter(getSampleRate(), frequencies[band], q, juce::Decibels::decibelsToGain(newGain));
                filterL->setCoefficients(coeffs);
                filterR->setCoefficients(coeffs);
            }
        }
    }

    // Process filters
    for (int band = 0; band < numBands; ++band)
    {
        if (std::abs(gains[band]) >= 0.01f) // Only process active filters
        {
            filters[band * 2].processSamples(leftChannel, numSamples);
            filters[band * 2 + 1].processSamples(rightChannel, numSamples);
        }
    }
}

juce::AudioProcessorEditor* GraphicEQ15BandAudioProcessor::createEditor()
{
    return new GraphicEQ15BandAudioProcessorEditor(*this);
}

void GraphicEQ15BandAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GraphicEQ15BandAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GraphicEQ15BandAudioProcessor();
}