set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

cpmaddpackage(
  NAME
  PORTAUDIO
  GITHUB_REPOSITORY
  PortAudio/portaudio
  VERSION
  19.7.0
  EXCLUDE_FROM_ALL
  TRUE
)

add_executable(music_player source/main.cpp)

target_link_libraries(music_player PRIVATE portaudio_static)
target_compile_options(music_player PRIVATE "${PROJECT_WARNINGS_CXX}")

