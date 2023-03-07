list(APPEND DSPAPPLE_SOURCES
  "catch_amalgamated.cpp"
  "fir.cpp"
  "fm_demod_test.cpp"
  "resample.cpp"
)

add_executable(dspapple_test ${DSPAPPLE_SOURCES})
target_link_libraries(dspapple_test PRIVATE DSPApple)
add_test(NAME DSPAPPLE_TEST COMMAND dspapple_test WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
