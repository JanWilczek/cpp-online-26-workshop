#include <utility>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <ranges>
#include <numbers>
#include <cmath>
#include <span>

namespace pa_ex {
class Initializer {
public:
  Initializer() : _error{Pa_Initialize()} {}

  ~Initializer() {
    if (_error && *_error == paNoError) {
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
      : _error{std::exchange(other._error, std::nullopt)} {}

  Initializer& operator=(Initializer&& other) noexcept {
    std::swap(_error, other._error);
    return *this;
  }

private:
  std::optional<PaError> _error;
};

class Stream {
public:
  template <typename... Ts>
  explicit Stream(Ts&&... args)
      : _error{Pa_OpenDefaultStream(&_stream, std::forward<Ts>(args)...)} {}

  ~Stream() {
    if (_stream != nullptr && _error == paNoError) {
      Pa_CloseStream(_stream);
    }
  }

  Stream(const Stream& other) = delete;
  Stream& operator=(const Stream& other) = delete;

  Stream(Stream&& other) noexcept
      : _stream{std::exchange(other._stream, nullptr)},
        _error{std::exchange(other._error, paNoError)} {}

  Stream& operator=(Stream&& other) noexcept {
    std::swap(_stream, other._stream);
    std::swap(_error, other._error);
    return *this;
  }

  void start() { Pa_StartStream(_stream); }

  void stop() { Pa_StopStream(_stream); }

private:
  PaStream* _stream{nullptr};
  PaError _error{paNoError};
};
}  // namespace pa_ex

class MusicPlayer {
public:
  MusicPlayer()
      : _stream{inputChannelCount,
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
                this} {}

  void start() { _stream.start(); }
  void stop() { _stream.stop(); }

private:
  static constexpr auto inputChannelCount = 0;
  static constexpr auto outputChannelCount = 2;
  static constexpr auto outputChannelCountUnsigned =
      static_cast<size_t>(outputChannelCount);
  static constexpr auto sampleRate = 44100.;

  int audioCallback(const void* /* input */,
                    void* output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* /* timeInfo */,
                    PaStreamCallbackFlags /* statusFlags */) {
    const auto sampleCount = frameCount * outputChannelCountUnsigned;
    auto buffer = std::span<float>{static_cast<float*>(output),
                                   sampleCount};  // interleaved samples

    for ([[maybe_unused]] const auto i : std::views::iota(0u, frameCount)) {
      constexpr auto amplitude = 0.25f;
      const auto outputSample = amplitude * std::sin(_phase);

      for (const auto channel :
           std::views::iota(0u, outputChannelCountUnsigned)) {
        buffer[(outputChannelCountUnsigned * i) + channel] = outputSample;
      }

      constexpr auto frequency = 220.f;
      _phase += 2 * std::numbers::pi_v<float> * frequency /
                static_cast<float>(sampleRate);
    }

    return paContinue;
  }

  pa_ex::Initializer _initializer;
  pa_ex::Stream _stream;
  float _phase = 0.f;
};

int main() {
  std::cout << "PortAudio version:" << Pa_GetVersionInfo()->versionText;

  MusicPlayer player;
  player.start();

  Pa_Sleep(3L * 1000L);

  player.stop();
}
