#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <print>
#include <ranges>
#include <numbers>
#include <cmath>
#include <mdspan>
#include <filesystem>
#include <AudioFile.h>

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

class AudioProcessor {
public:
  AudioProcessor() = default;
  virtual ~AudioProcessor() = default;
  AudioProcessor(const AudioProcessor&) = delete;
  AudioProcessor& operator=(const AudioProcessor&) = delete;
  AudioProcessor(AudioProcessor&&) = delete;
  AudioProcessor& operator=(AudioProcessor&&) = delete;

  virtual void prepareToPlay([[maybe_unused]] double sampleRate) {}

  using AudioBuffer =
      std::mdspan<float, std::dextents<int, 2>, std::layout_left>;
  virtual void processBlock(AudioBuffer) = 0;
};

class SineGenerator : public AudioProcessor {
public:
  SineGenerator() = default;

  void prepareToPlay(double sampleRate) override {
    _sampleRate = static_cast<float>(sampleRate);
  }

  void processBlock(AudioBuffer buffer) override {
    for (const auto frame : std::views::iota(0, buffer.extent(1))) {
      constexpr auto amplitude = 0.25f;
      const auto outputSample = amplitude * std::sin(_phase);

      for (const auto channel : std::views::iota(0, buffer.extent(0))) {
        buffer[channel, frame] = outputSample;
      }

      constexpr auto frequency = 220.f;
      _phase += 2 * std::numbers::pi_v<float> * frequency / _sampleRate;
    }
  }

private:
  float _phase = 0.f;
  float _sampleRate = 0.f;
};

class FilePlayer : public AudioProcessor {
public:
  explicit FilePlayer(std::filesystem::path filepath) {
    _file.load(filepath.string());
  }

  void processBlock(AudioBuffer buffer) override {
    const auto channelCount =
        std::min(buffer.extent(0), _file.getNumChannels());

    for (const auto frame : std::views::iota(0, buffer.extent(1))) {
      if (_playhead < static_cast<size_t>(_file.getNumSamplesPerChannel())) {
        for (const auto channel : std::views::iota(0, channelCount)) {
          buffer[channel, frame] =
              _file.samples[static_cast<size_t>(channel)][_playhead];
        }
        _playhead++;
      }
    }
  }

private:
  AudioFile<float> _file;
  size_t _playhead = 0u;
};

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
                this} {
    _processor.prepareToPlay(sampleRate);
  }

  void start() { _stream.start(); }
  void stop() { _stream.stop(); }

private:
  static constexpr auto inputChannelCount = 0;
  static constexpr auto outputChannelCount = 2;
  static constexpr auto sampleRate = 44100.;

  int audioCallback(const void* /* input */,
                    void* output,
                    unsigned long frameCount,
                    const PaStreamCallbackTimeInfo* /* timeInfo */,
                    PaStreamCallbackFlags /* statusFlags */) {
    auto buffer = AudioProcessor::AudioBuffer{static_cast<float*>(output),
                                              outputChannelCount, frameCount};

    _processor.processBlock(buffer);

    return paContinue;
  }

  pa_ex::Initializer _initializer;
  pa_ex::Stream _stream;
  // SineGenerator _processor;
  FilePlayer _processor{"/Users/jawi/Music/TestSignals/Guitar_5th.wav"};
};

int main() {
  std::println("PortAudio version: {}", Pa_GetVersionInfo()->versionText);

  MusicPlayer player;
  player.start();

  Pa_Sleep(3L * 1000L);

  player.stop();
}
