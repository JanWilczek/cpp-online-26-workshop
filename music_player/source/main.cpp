#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <print>
#include <fx/fx.h>

namespace pa_ex {
class Initializer {
public:
  Initializer() : error_{Pa_Initialize()} {}

  ~Initializer() {
    if (error_ && *error_ == paNoError) {
      Pa_Terminate();
    }
  }

  Initializer(const Initializer&) : Initializer() {}

  Initializer& operator=(const Initializer& other) {
    if (this != &other) {
      *this = Initializer{};
    }
    return *this;
  }

  Initializer(Initializer&& other) noexcept
      : error_{std::exchange(other.error_, std::nullopt)} {}

  Initializer& operator=(Initializer&& other) noexcept {
    std::swap(error_, other.error_);
    return *this;
  }

private:
  std::optional<PaError> error_;
};

class Stream {
public:
  template <typename... Ts>
  explicit Stream(Ts&&... args)
      : error_{Pa_OpenDefaultStream(&stream_, std::forward<Ts>(args)...)} {}

  ~Stream() {
    if (stream_ != nullptr && error_ == paNoError) {
      Pa_CloseStream(stream_);
    }
  }

  Stream(const Stream& other) = delete;
  Stream& operator=(const Stream& other) = delete;

  Stream(Stream&& other) noexcept
      : stream_{std::exchange(other.stream_, nullptr)},
        error_{std::exchange(other.error_, paNoError)} {}

  Stream& operator=(Stream&& other) noexcept {
    std::swap(stream_, other.stream_);
    std::swap(error_, other.error_);
    return *this;
  }

  void start() { Pa_StartStream(stream_); }

  void stop() { Pa_StopStream(stream_); }

private:
  PaStream* stream_{nullptr};
  PaError error_{paNoError};
};
}  // namespace pa_ex

class MusicPlayer {
public:
  explicit MusicPlayer(
      std::vector<std::unique_ptr<fx::AudioProcessor>> processors)
      : stream_{inputChannelCount,
                outputChannelCount,
                paFloat32,
                sampleRate,
                static_cast<unsigned long>(paFramesPerBufferUnspecified),
                [](const void* input,
                   void* output,
                   unsigned long frameCount,
                   const PaStreamCallbackTimeInfo* timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void* userData) {
                  auto* thisPtr = static_cast<MusicPlayer*>(userData);
                  return thisPtr->audioCallback(input, output, frameCount,
                                                timeInfo, statusFlags);
                },
                this},
        processors_{std::move(processors)} {
    const auto ret = std::ranges::remove_if(
        processors_, [](auto& p) { return p.get() == nullptr; });
    processors_.erase(ret.begin(), ret.end());
    for (auto& processor : processors_) {
      processor->prepareToPlay(
          sampleRate,
          static_cast<int>(
              sampleRate) /* buffer sizes longer than 1 second are rare */,
          outputChannelCount);
    }
  }

  void start() { stream_.start(); }
  void stop() { stream_.stop(); }

private:
  static constexpr auto inputChannelCount = 0;
  static constexpr auto outputChannelCount = 2;
  static constexpr auto sampleRate = 44100.;

  int audioCallback(const void* /* input */,
                    void* output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* /* timeInfo */,
                    PaStreamCallbackFlags /* statusFlags */) {
    auto buffer = fx::AudioProcessor::AudioBuffer{
        static_cast<float*>(output), outputChannelCount, frameCount};

    for (auto& processor : processors_) {
      processor->processBlock(buffer);
    }

    return paContinue;
  }

  pa_ex::Initializer initializer_;
  pa_ex::Stream stream_;
  std::vector<std::unique_ptr<fx::AudioProcessor>> processors_;
};

int main() {
  std::println("PortAudio version: {}", Pa_GetVersionInfo()->versionText);

  MusicPlayer player{[] {
    std::vector<std::unique_ptr<fx::AudioProcessor>> processors;
    processors.push_back(std::make_unique<fx::FilePlayer>(
        "/Users/jawi/Music/TestSignals/Guitar_5th.wav"));
    processors.push_back(std::make_unique<fx::Flanger>());
    return processors;
  }()};
  player.start();

  Pa_Sleep(5L * 1000L);

  player.stop();
}
