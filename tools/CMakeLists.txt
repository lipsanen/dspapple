add_executable(audioplayer "audioplayer.cpp")
add_executable(fmdemod "fm_demod.cpp")
add_executable(resampler "resampler.cpp")
add_executable(wavegen "wavegen.cpp")

target_link_libraries(audioplayer PUBLIC SDL2)
target_link_libraries(fmdemod PUBLIC DSPApple)
target_link_libraries(resampler PUBLIC DSPApple)

