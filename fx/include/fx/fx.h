#pragma once
#include <vector>
#include <mdspan>
#include <numbers>
#include <cmath>
#include <ranges>
#include <filesystem>
// TODO: Include AudioFile.h
#include <wolfsound/common/wolfsound_Frequency.hpp>
#include <wolfsound/dsp/wolfsound_FractionalDelayLine.hpp>

namespace fx {
using AudioBuffer = std::mdspan<float, std::dextents<int, 2>, std::layout_left>;

class SineGenerator {
public:
  SineGenerator() = default;

  void setFrequency(wolfsound::Frequency f) { frequency_ = f; }

  void prepareToPlay(double sampleRate) {
    sampleRate_ = static_cast<float>(sampleRate);
  }

  void processBlock(AudioBuffer buffer) {
    for (const auto frame : std::views::iota(0, buffer.extent(1))) {
      constexpr auto amplitude = 0.25f;
      const auto outputSample = amplitude * std::sin(phase_);

      for (const auto channel : std::views::iota(0, buffer.extent(0))) {
        buffer[channel, frame] = outputSample;
      }

      phase_ += getPhaseIncrement();

      if (twoPi < phase_) {
        phase_ -= twoPi;
      }
    }
  }

private:
  static constexpr auto twoPi = 2.f * std::numbers::pi_v<float>;

  [[nodiscard]] float getPhaseIncrement() const noexcept {
    return twoPi * frequency_.value() / sampleRate_;
  }

  float phase_ = 0.f;
  float sampleRate_ = 0.f;
  wolfsound::Frequency frequency_{220.f};
};

class FilePlayer {
public:
  explicit FilePlayer([[maybe_unused]] const std::filesystem::path& filepath) {
    // TODO: Load file
  }

  void processBlock(AudioBuffer buffer) {
    for ([[maybe_unused]] const auto frame :
         std::views::iota(0, buffer.extent(1))) {
      // TODO: Fill the played back channels with channel data from file
      // TODO: Fill the played back channels that aren't in the file with
      // silence
      // TODO: Fill the samples beyond file length with zeros
      // TODO: Update playhead_
    }
  }

private:
  // TODO: Instantiate AudioFile
  [[maybe_unused]] size_t playhead_ = 0u;
};

class Flanger {
public:
  struct Parameters {
    wolfsound::Frequency lfoFrequency{0.1f};
  };

  explicit Flanger() { /* TODO: Call setParameters() with default parameters */
  }

  void prepareToPlay(double sampleRate,
                     int maxFramesPerBuffer,
                     int channelCount) {
    channelProcessors_.resize(static_cast<size_t>(channelCount));
    for (auto& p : channelProcessors_) {
      p.prepareToPlay(sampleRate);
    }

    // TODO: Prepare the LFO

    lfoBuffer_.resize(static_cast<size_t>(maxFramesPerBuffer));
    std::ranges::fill(lfoBuffer_, 0.f);
  }

  void setParameters([[maybe_unused]] const Parameters& newParameters) {
    // TODO: Set LFO frequency
  }

  void processBlock(AudioBuffer buffer) {
    WS_ASSERT(buffer.extent(0) <= std::ssize(channelProcessors_),
              "too many channels than the effect can handle");
    WS_ASSERT(buffer.extent(1) <= std::ssize(lfoBuffer_),
              "the host is misbehaving");

    // TODO: Generate the LFO signal to the lfoBuffer_

    // Process samples one by one, at least initially.
    using namespace std::views;
    for (const auto channel : iota(0, buffer.extent(0))) {
      for (const auto frame : iota(0, buffer.extent(1))) {
        const auto processedSample =
            channelProcessors_[static_cast<size_t>(channel)].processSample(
                buffer[channel, frame], lfoBuffer_[static_cast<size_t>(frame)]);
        buffer[channel, frame] = processedSample;
      }
    }
  }

private:
  class ChannelProcessor {
  public:
    void prepareToPlay([[maybe_unused]] double sampleRate) {
      [[maybe_unused]] constexpr auto maxDelaySeconds = 0.002;
      // TODO: Calculate the middleDelay_ and maxDelay_ in samples
      delayLine_.reset();
    }

    float processSample(float sample, [[maybe_unused]] float lfoSample) {
      const auto& x = sample;

      // TODO: Implement the flanger difference equation

      const auto y = x;

      return y;
    }

  private:
    static constexpr auto feedforward = 0.7f;
    static constexpr auto feedback = 0.7f;
    static constexpr auto blend = 0.7f;

    [[maybe_unused]] float maxDelay_ = 0.f;
    [[maybe_unused]] float middleDelay_ = 0.f;
    wolfsound::FractionalDelayLine<float> delayLine_;
  };

  // TODO: Add LFO generator
  std::vector<float> lfoBuffer_;
  std::vector<ChannelProcessor> channelProcessors_;
  Parameters parameters_;
};
}  // namespace fx
