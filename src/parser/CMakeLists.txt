add_library(parser "")

target_sources(parser
  PRIVATE
    adder.cpp
    parser.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/adder.h
    ${CMAKE_CURRENT_LIST_DIR}/parser.h
  )

target_include_directories(parser
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )