add_executable(bench 
    "cpu_conversion.cpp"
    "demod_bench.cpp" 
    "fir_bench.cpp")
target_link_libraries(bench PUBLIC DSPApple benchmark)
