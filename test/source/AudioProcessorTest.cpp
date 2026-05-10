#include <audio_plugin/audio_plugin.h>
#include <gtest/gtest.h>
#include <wolfsound/test/wolfsound_ProcessorFileIoTest.hpp>

namespace audio_plugin {
namespace {
struct Processor {
  void prepare(const juce::dsp::ProcessSpec& spec) {
    processor.prepareToPlay(spec.sampleRate,
                            static_cast<int>(spec.maximumBlockSize));
    buffer.setSize(static_cast<int>(spec.numChannels),
                   static_cast<int>(spec.maximumBlockSize));
    // processor.getParameterRefs().lfoFrequency = 7.f;
  }

  void process(juce::dsp::ProcessContextReplacing<float> context) {
    const auto inputBlock = context.getInputBlock();
    inputBlock.copyTo(buffer);
    juce::MidiBuffer midiBuffer;

    processor.processBlock(buffer, midiBuffer);

    context.getOutputBlock().copyFrom(buffer);
  }

  PluginProcessor processor;
  juce::AudioBuffer<float> buffer;
};
}  // namespace

TEST(AudioProcessor, End2EndTest) {
  wolfsound::ProcessorFileIoTest<Processor> test{{
      .inputAudioFile = "Guitar_5th.wav",
      .name = "FlangerEnd2EndTest",
      .audioInputFilesDirectoryPath = (std::filesystem::path{__FILE__}
                                           .parent_path()
                                           .parent_path()
                                           .parent_path() /
                                       "data")
                                          .string(),
  }};

  test.run();
}
}  // namespace audio_plugin
