namespace audio_plugin {
namespace {
/** @brief This is taken straight from JUCE's WebViewPluginDemo.h */
template <typename Param, typename Group, typename... Ts>
Param& addToLayout(Group& layout, Ts&&... ts) {
  auto param = std::make_unique<Param>(std::forward<Ts>(ts)...);
  auto& ref = *param;
  layout.add(std::move(param));
  return ref;
}
}  // namespace

namespace id {
static const juce::ParameterID LFO_FREQUENCY_HZ{"lfoFrequencyHz", 1};
}

PluginProcessor::PluginProcessor(
    juce::AudioProcessorValueTreeState::ParameterLayout parameterLayout)
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      parameters_{parameterLayout},
      apvts_{*this, nullptr, "FLANGERPARAMS", std::move(parameterLayout)} {
}
const juce::String PluginProcessor::getName() const {
  return JUCE_PLUGIN_NAME;
}

bool PluginProcessor::acceptsMidi() const {
#if JUCE_NEEDS_MIDI_INPUT
  return true;
#else
  return false;
#endif
}

bool PluginProcessor::producesMidi() const {
#if JUCE_NEEDS_MIDI_OUTPUT
  return true;
#else
  return false;
#endif
}

bool PluginProcessor::isMidiEffect() const {
#if JUCE_IS_MIDI_EFFECT
  return true;
#else
  return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int PluginProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int PluginProcessor::getCurrentProgram() {
  return 0;
}

void PluginProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String PluginProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void PluginProcessor::changeProgramName(int index,
                                        const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  flanger_.prepare(juce::dsp::ProcessSpec{
      .sampleRate = sampleRate,
      .maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock),
      .numChannels = static_cast<juce::uint32>(getTotalNumInputChannels()),
  });
}

void PluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  flanger_.reset();
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
#if JUCE_IS_MIDI_EFFECT
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JUCE_IS_SYNTH
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                   juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);

  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  Flanger<SampleType>::Parameters newParameters{
      .lfoFrequency = wolfsound::Frequency{parameters_.lfoFrequency.get()},
  };
  *flanger_.state = newParameters;

  juce::dsp::AudioBlock<SampleType> audioBlock{buffer};
  flanger_.process(juce::dsp::ProcessContextReplacing<SampleType>{audioBlock});
}

bool PluginProcessor::hasEditor() const {
  return true;  // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
  return new PluginEditor(*this);
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  juce::ignoreUnused(destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::ignoreUnused(data, sizeInBytes);
}

PluginProcessor::Parameters::Parameters(
    juce::AudioProcessorValueTreeState::ParameterLayout& layout)
    : lfoFrequency{addToLayout<juce::AudioParameterFloat>(
          layout,
          id::LFO_FREQUENCY_HZ,
          "LFO frequency",
          juce::NormalisableRange<float>{0.01f, 10.f, 0.01f},
          Flanger<SampleType>::Parameters{}.lfoFrequency.value(),
          juce::AudioParameterFloatAttributes{}.withLabel("Hz"))} {}
}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::PluginProcessor({});
}
