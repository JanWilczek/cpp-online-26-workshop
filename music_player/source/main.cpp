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
  explicit MusicPlayer(/* TODO: Pass the processing chain */)
  // TODO: Initialize the stream with no input channels, 2 output channels,
  // 44.1 kHz sampling rate, and audioCallback() as the callback
  {}

  void start() { /* TODO: Start the stream. */ }
  void stop() { /* TODO: Stop the stream. */ }

private:
  static constexpr auto inputChannelCount = 0;
  static constexpr auto outputChannelCount = 2;
  static constexpr auto sampleRate = 44100.;

  int audioCallback(const void* /* input */,
                    void* output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* /* timeInfo */,
                    PaStreamCallbackFlags /* statusFlags */) {
    [[maybe_unused]] auto buffer = fx::AudioBuffer{
        static_cast<float*>(output), outputChannelCount, frameCount};

    // TODO: Fill all output channels with a 220 Hz sine

    return paContinue;
  }

  // TODO: Initialize PortAudio
  // TODO: Add a stream
};

int main() {
  std::println("PortAudio version: {}", Pa_GetVersionInfo()->versionText);

  MusicPlayer player;
  player.start();

  Pa_Sleep(5L * 1000L);

  player.stop();
}
