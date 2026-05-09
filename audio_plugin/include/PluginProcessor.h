#pragma once

namespace audio_plugin {
class PluginProcessor : public juce::AudioProcessor {
public:
  using SampleType = float;

  explicit PluginProcessor(
      wolfsound::JuceParameterHolder::Builder builder = {});

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  struct Parameters {
    explicit Parameters(wolfsound::JuceParameterHolder::Builder&);
    // TODO: Add LFO frequency parameter (as a reference)
  };
  const Parameters& getParameterRefs() const;

private:
  using ParameterLayout = juce::AudioProcessorValueTreeState::ParameterLayout;

  Parameters parameters_;
  wolfsound::JuceParameterHolder parameterHolder_;
  // TODO: Add flanger
  std::vector<float> interleavedBuffer_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
}  // namespace audio_plugin
