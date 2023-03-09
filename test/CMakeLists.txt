list(APPEND DSPAPPLE_SOURCES
  "conversion.cpp"
  "fir.cpp"
  "fm_demod_test.cpp"
  "resample.cpp"
)

if(DSPAppleCuda)
  list(APPEND DSPAPPLE_SOURCES
    "cuda/tests.cu"
  )
endif()

add_executable(dspapple_test ${DSPAPPLE_SOURCES})
target_link_libraries(dspapple_test PRIVATE DSPApple gtest gtest_main)
add_test(NAME DSPAPPLE_TEST COMMAND dspapple_test WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
