cpmaddpackage(
  NAME
  AUDIO_FILE
  GITHUB_REPOSITORY
  adamstark/AudioFile
  GIT_TAG
  1.1.4
)

add_library(fx INTERFACE)
target_include_directories(fx INTERFACE ${AUDIO_FILE_SOURCE_DIR} include)
target_link_libraries(fx INTERFACE wolfsound::wolfsound_dsp_utils)
add_library(my_company::fx ALIAS fx)
