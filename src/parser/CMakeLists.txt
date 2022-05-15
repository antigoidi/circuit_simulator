add_library(parser "")

target_sources(parser
  PRIVATE
    adder.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/adder.h
  )

target_include_directories(parser
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )