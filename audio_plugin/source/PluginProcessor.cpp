namespace audio_plugin {
namespace {
void interleave(juce::AudioBuffer<float>& src, std::span<float> dest) {
  using namespace std::views;

  jassert(static_cast<size_t>(src.getNumChannels() * src.getNumSamples()) <=
          dest.size());

  for (const auto channel : iota(0, src.getNumChannels())) {
    for (const auto sample : iota(0, src.getNumSamples())) {
      const auto destIndex =
          static_cast<size_t>(sample * src.getNumChannels() + channel);
      dest[destIndex] = src.getSample(channel, sample);
    }
  }
}

void deinterleave(std::span<float> src, juce::AudioBuffer<float>& dst) {
  using namespace std::views;

  jassert(static_cast<size_t>(dst.getNumChannels() * dst.getNumSamples()) <=
          src.size());

  for (const auto channel : iota(0, dst.getNumChannels())) {
    for (const auto sample : iota(0, dst.getNumSamples())) {
      const auto srcIndex =
          static_cast<size_t>(sample * dst.getNumChannels() + channel);
      dst.setSample(channel, sample, src[srcIndex]);
    }
  }
}
}  // namespace

PluginProcessor::PluginProcessor(
    wolfsound::JuceParameterHolder::Builder builder)
    : AudioProcessor(
          BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
              ),
      parameters_{builder},
      parameterHolder_{std::move(builder).build(*this)} {
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

void PluginProcessor::prepareToPlay([[maybe_unused]] double sampleRate,
                                    int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need.
  const auto maxChannels =
      std::max(getTotalNumInputChannels(), getTotalNumOutputChannels());
  // TODO: Prepare the flanger
  interleavedBuffer_.resize(static_cast<size_t>(samplesPerBlock * maxChannels));
}

void PluginProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
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

  // TODO: Pass LFO modulation rate parameter value to flanger

  interleave(buffer, interleavedBuffer_);
  // TODO: Process audio with flanger
  deinterleave(interleavedBuffer_, buffer);
}

bool PluginProcessor::hasEditor() const {
  // TODO: Change this to true, when you are ready to work on the editor
  return false;
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() {
  // TODO: Instantiate the editor
  return nullptr;
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
  const auto serializedParameters = wolfsound::SerializedParameters::from(
      wolfsound::toVarArray(parameterHolder_));
  if (serializedParameters.has_value()) {
    juce::MemoryOutputStream memory{destData, true};
    juce::JSON::writeToStream(memory, serializedParameters->toVar());
  }
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  juce::MemoryInputStream inputStream{data, static_cast<size_t>(sizeInBytes),
                                      false};
  const auto deserializedParameters = juce::JSON::parse(inputStream);
  const auto parameters =
      wolfsound::SerializedParameters::from(deserializedParameters);
  if (parameters.has_value()) {
    wolfsound::update(parameterHolder_, parameters->toVarArray());
  }
}

auto PluginProcessor::getParameterRefs() const -> const Parameters& {
  return parameters_;
}

PluginProcessor::Parameters::Parameters(
    [[maybe_unused]] wolfsound::JuceParameterHolder ::Builder& builder)
// TODO: Instantiate LFO frequency parameter using builder.add<>()
{}
}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::PluginProcessor();
}
