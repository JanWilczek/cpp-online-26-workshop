#include <audio_plugin/audio_plugin.h>
#include <gtest/gtest.h>
#include <juce_events/juce_events.h>

namespace audio_plugin {
TEST(AudioProcessor, Foo) {
  juce::ScopedJuceInitialiser_GUI guiInitializer{};
  PluginProcessor processor{{}};
}
}  // namespace audio_plugin
