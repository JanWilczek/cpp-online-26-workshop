#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <iostream>
#include <ranges>
#include <numbers>
#include <cmath>

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
    if (_error == paNoError) {
      Pa_CloseStream(_stream);
    }
  }

  // copy & move constructors & assignment operators

private:
  PaStream* _stream{nullptr};
  PaError _error{paNoError};
};
}  // namespace pa_ex

class MusicPlayer {
public:
  // TODO: add play() and stop() that call Pa_StartStream and Pa_StopStream
  // respectively

  MusicPlayer()
      : _stream{0,
                2,
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

private:
  static constexpr auto sampleRate = 44100.;

  int audioCallback(const void* /* input */,
                    void* output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* /* timeInfo */,
                    PaStreamCallbackFlags /* statusFlags */) {
    auto* out = static_cast<float*>(output);  // interleaved samples

    for ([[maybe_unused]] const auto i : std::views::iota(0u, frameCount)) {
      constexpr auto amplitude = 0.25f;
      *out++ = amplitude * std::sin(phase);
      *out++ = *(out - 1);

      phase += 2 * std::numbers::pi_v<float> * 220.f /
               static_cast<float>(sampleRate);
    }

    return paContinue;
  }

  pa_ex::Initializer _initializer;
  pa_ex::Stream _stream;
  float phase = 0.f;
};

int main() {
  std::cout << "PortAudio version:" << Pa_GetVersionInfo()->versionText;

  MusicPlayer player;
}
