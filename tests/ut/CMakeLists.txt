option(ENABLE_UNIT_TESTS "Enable unit tests" ON)
message(STATUS "Enable testing: ${ENABLE_UNIT_TESTS}")

if(ENABLE_UNIT_TESTS)
  add_executable(cpp_test "")

  target_sources(cpp_test
    PRIVATE
      example.cpp
    )

  target_link_libraries(cpp_test
    PRIVATE
      parser
      gtest_main
    )

  enable_testing()

  add_test(
    NAME google_test
    COMMAND $<TARGET_FILE:cpp_test>
    )
endif()