#pragma once
#include <mdspan>
#include <numbers>
#include <cmath>
#include <ranges>
#include <filesystem>
#include <AudioFile.h>
#include <wolfsound/common/wolfsound_Frequency.hpp>
#include <wolfsound/dsp/wolfsound_FractionalDelayLine.hpp>

namespace fx {
class AudioProcessor {
public:
  AudioProcessor() = default;
  virtual ~AudioProcessor() = default;
  AudioProcessor(const AudioProcessor&) = delete;
  AudioProcessor& operator=(const AudioProcessor&) = delete;
  AudioProcessor(AudioProcessor&&) = delete;
  AudioProcessor& operator=(AudioProcessor&&) = delete;

  virtual void prepareToPlay([[maybe_unused]] double sampleRate,
                             [[maybe_unused]] int maxFramesPerBuffer,
                             [[maybe_unused]] int channelCount) {}

  using AudioBuffer =
      std::mdspan<float, std::dextents<int, 2>, std::layout_left>;
  virtual void processBlock(AudioBuffer) = 0;
};

class SineGenerator : public AudioProcessor {
public:
  SineGenerator() = default;

  void setFrequency(wolfsound::Frequency f) { frequency_ = f; }

  void prepareToPlay(double sampleRate, int, int) override {
    sampleRate_ = static_cast<float>(sampleRate);
  }

  void processBlock(AudioBuffer buffer) override {
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

class FilePlayer : public AudioProcessor {
public:
  explicit FilePlayer(const std::filesystem::path& filepath) {
    file_.load(filepath.string());
  }

  void processBlock(AudioBuffer buffer) override {
    const auto channelCount =
        std::min(buffer.extent(0), file_.getNumChannels());

    for (const auto frame : std::views::iota(0, buffer.extent(1))) {
      if (playhead_ < static_cast<size_t>(file_.getNumSamplesPerChannel())) {
        for (const auto channel : std::views::iota(0, channelCount)) {
          buffer[channel, frame] =
              file_.samples[static_cast<size_t>(channel)][playhead_];
        }
        playhead_++;
      }
    }
  }

private:
  AudioFile<float> file_;
  size_t playhead_ = 0u;
};

class Flanger : public AudioProcessor {
public:
  struct Parameters {
    wolfsound::Frequency lfoFrequency{0.1f};
  };

  explicit Flanger() { lfo_.setFrequency(parameters_.lfoFrequency); }

  void prepareToPlay(double sampleRate,
                     int maxFramesPerBuffer,
                     int channelCount) override {
    channelProcessors_.resize(static_cast<size_t>(channelCount));
    for (auto& p : channelProcessors_) {
      p.prepareToPlay(sampleRate);
    }

    lfo_.prepareToPlay(sampleRate, maxFramesPerBuffer, 1u);

    lfoBuffer_.resize(static_cast<size_t>(maxFramesPerBuffer));
    std::ranges::fill(lfoBuffer_, 0.f);
  }

  void setParameters(const Parameters& newParameters) {
    lfo_.setFrequency(newParameters.lfoFrequency);
  }

  void processBlock(AudioBuffer buffer) override {
    WS_ASSERT(buffer.extent(0) <= std::ssize(channelProcessors_),
              "too many channels than the effect can handle");
    // Generate the LFO
    WS_ASSERT(buffer.extent(1) <= std::ssize(lfoBuffer_),
              "the host is misbehaving");
    lfo_.processBlock(
        AudioBuffer{lfoBuffer_.data(), 1, std::ssize(lfoBuffer_)});

    // Process samples one by one, at least initially.
    using namespace std::views;
    for (const auto channel : iota(0, buffer.extent(0))) {
      for (const auto sample : iota(0, buffer.extent(1))) {
        const auto processedSample =
            channelProcessors_[static_cast<size_t>(channel)].processSample(
                buffer[channel, sample],
                lfoBuffer_[static_cast<size_t>(sample)]);
        buffer[channel, sample] = processedSample;
      }
    }
  }

private:
  class ChannelProcessor {
  public:
    void prepareToPlay(double sampleRate) {
      constexpr auto maxDelaySeconds = 0.002;
      maxDelay_ = static_cast<float>(std::ceil(sampleRate * maxDelaySeconds));
      middleDelay_ = maxDelay_ / 2.f;
      delayLine_.reset();
    }

    float processSample(float sample, float lfoSample) {
      const auto& x = sample;
      const auto xh = x + (feedback_ * delayLine_.popSample(middleDelay_));

      const auto lfoUnipolarValue = (lfoSample + 1) / 2;
      const auto currentDelay = lfoUnipolarValue * maxDelay_;

      const auto y =
          (blend_ * xh) + (feedforward_ * delayLine_.popSample(currentDelay));

      delayLine_.pushSample(xh);

      return y;
    }

  private:
    static constexpr auto feedforward_ = 0.7f;
    static constexpr auto feedback_ = 0.7f;
    static constexpr auto blend_ = 0.7f;

    float maxDelay_ = 0.f;
    float middleDelay_ = 0.f;
    wolfsound::FractionalDelayLine<float> delayLine_;
  };

  SineGenerator lfo_;
  std::vector<float> lfoBuffer_;
  std::vector<ChannelProcessor> channelProcessors_;
  Parameters parameters_;
};
}  // namespace fx
