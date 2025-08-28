#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include <JuceHeader.h>

class GraphicEQ15BandAudioProcessor : public juce::AudioProcessor
{
public:
    GraphicEQ15BandAudioProcessor();
    ~GraphicEQ15BandAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    static constexpr int numBands = 15;
    float frequencies[numBands] = { 25.0f, 40.0f, 63.0f, 100.0f, 160.0f, 250.0f, 400.0f, 630.0f, 
                                    1000.0f, 1600.0f, 2500.0f, 4000.0f, 6300.0f, 10000.0f, 16000.0f };
    juce::AudioProcessorValueTreeState parameters;
    std::array<juce::IIRFilter, numBands * 2> filters; // Stereo: 15 bands x 2 channels
    std::array<float, numBands> gains;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GraphicEQ15BandAudioProcessor)
};

#endif // PLUGINPROCESSOR_H_INCLUDED