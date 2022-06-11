add_library(matrix "")

target_sources(matrix
  PRIVATE
    matrix.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/matrix.h
  )

target_include_directories(matrix
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(matrix
  PRIVATE
    parser
  )