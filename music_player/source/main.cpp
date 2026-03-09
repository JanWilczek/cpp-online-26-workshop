#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <expected>
#include <iostream>
#include <memory>
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
}  // namespace pa_ex

class MusicPlayer {
public:
  static std::expected<std::unique_ptr<MusicPlayer>, PaError> create() {
    const auto error = Pa_Initialize();

    if (error != paNoError) {
      return std::unexpected{error};
    }

    return std::unique_ptr<MusicPlayer>(new MusicPlayer);
  }

  ~MusicPlayer() { Pa_Terminate(); }

  MusicPlayer(const MusicPlayer&) = delete;
  const MusicPlayer& operator=(const MusicPlayer&) = delete;
  MusicPlayer(MusicPlayer&&) = delete;
  MusicPlayer&& operator=(MusicPlayer&&) = delete;

  // TODO: add play() and stop() that call Pa_StartStream and Pa_StopStream
  // respectively
private:
  static constexpr auto sampleRate = 44100.;

  MusicPlayer() {
    error = Pa_OpenDefaultStream(
        &stream, 0, 2, paFloat32, sampleRate, paFramesPerBufferUnspecified,
        [](const void* input, void* output, unsigned long frameCount,
           const PaStreamCallbackTimeInfo* timeInfo,
           PaStreamCallbackFlags statusFlags, void* userData) {
          auto* thisPtr = static_cast<MusicPlayer*>(userData);
          return thisPtr->audioCallback(input, output, frameCount, timeInfo,
                                        statusFlags);
        },
        this);
  }

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

  PaStream* stream{nullptr};
  PaError error = paNoError;
  float phase = 0.f;
};

int main() {
  std::cout << "PortAudio version:" << Pa_GetVersionInfo()->versionText;
  auto maybeMusicPlayer = MusicPlayer::create();

  if (!maybeMusicPlayer) {
    std::cout << "PortAudio error: "
              << Pa_GetErrorText(maybeMusicPlayer.error());
    return maybeMusicPlayer.error();
  }

  [[maybe_unused]] auto musicPlayer = std::move(maybeMusicPlayer.value());
}
