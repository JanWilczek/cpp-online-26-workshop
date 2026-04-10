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

set_target_properties(portaudio_static PROPERTIES COMPILE_WARNING_AS_ERROR OFF)
if(MSVC)
  target_compile_options(portaudio_static PRIVATE /w)
else()
  target_compile_options(portaudio_static PRIVATE -w)
endif()

add_executable(music_player source/main.cpp)

target_link_libraries(music_player PRIVATE portaudio_static my_company::fx)
target_compile_options(music_player PRIVATE "${PROJECT_WARNINGS_CXX}")
