add_executable(audioplayer "audioplayer.cpp")
add_executable(cosine_windows "cosine_windows.cpp")
add_executable(fmdemod "fm_demod.cpp")
add_executable(resampler "resampler.cpp")
add_executable(spectrum_analyzer "spectrum_analyzer.cpp")
add_executable(wavegen "wavegen.cpp")

target_link_libraries(audioplayer PUBLIC SDL2)
target_link_libraries(fmdemod PUBLIC DSPApple)
target_link_libraries(resampler PUBLIC DSPApple)
target_link_libraries(spectrum_analyzer PUBLIC DSPApple sfml-window sfml-system sfml-graphics)

