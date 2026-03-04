# Signals to CMake that we want to run tests from this directory.
enable_testing()

# Creates the test console application.
add_executable(AudioPluginTest source/AudioProcessorTest.cpp)

# Thanks to the fact that we link against the gtest_main library, we don't have to write the main function ourselves.
target_link_libraries(AudioPluginTest PRIVATE my_company::audio_plugin GTest::gtest_main)

# These definitions are recommended by JUCE:
target_compile_definitions(AudioPluginTest PRIVATE JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0)

# Adds googletest-specific CMake commands at our disposal.
include(GoogleTest)

# Add all tests defined with googletest to the CMake metadata
# so that these tests are run upon a call to ctest in the test
# projects' binary directory.
if(CMAKE_GENERATOR STREQUAL Xcode)
  # On macOS arm64, all binaries have to be signed before running.
  # In local development, the linker adds an ad-hoc placeholder signature.
  # In Xcode however, the ad-hoc signature is delayed until after the “Run Script” build phase,
  # so the POST_BUILD command added by gtest_discover_tests cannot run.
  # Thus, we need to delay test discovery until run time.
  # Source: https://discourse.cmake.org/t/googletest-crash-when-using-cmake-xcode-arm64/5766/8
  gtest_discover_tests(
    AudioPluginTest
    DISCOVERY_TIMEOUT
    60
    DISCOVERY_MODE
    PRE_TEST
  )
else()
  gtest_discover_tests(AudioPluginTest)
endif()
