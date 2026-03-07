#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <portaudio.h>
#pragma GCC diagnostic pop

#include <expected>
#include <iostream>
#include <memory>

class MusicPlayer {
public:
  static std::expected<std::unique_ptr<MusicPlayer>, PaError> create() {
    const auto error = Pa_Initialize();

    if (error != paNoError) {
      Pa_Terminate();
      return std::unexpected{error};
    }

    return std::unique_ptr<MusicPlayer>(new MusicPlayer);
  }

  ~MusicPlayer() { Pa_Terminate(); }

  MusicPlayer(const MusicPlayer&) = delete;
  const MusicPlayer& operator=(const MusicPlayer&) = delete;
  MusicPlayer(MusicPlayer&&) = delete;
  MusicPlayer&& operator=(MusicPlayer&&) = delete;

private:
  MusicPlayer() = default;
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
