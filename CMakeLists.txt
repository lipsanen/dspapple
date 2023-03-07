cmake_minimum_required(VERSION 3.10)

project(DSPApple VERSION 1.0.0)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -march=haswell -fsanitize=address")
set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=haswell -fno-omit-frame-pointer -ffast-math")

list(APPEND APPLE_SOURCES
     src/fm.cpp
     src/fir.cpp
     src/io.cpp
     src/resample.cpp
     )

add_library(DSPApple ${APPLE_SOURCES})
target_include_directories(DSPApple PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(DSPApple INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
enable_testing()
add_subdirectory(bench)
add_subdirectory(test)
add_subdirectory(tools)
