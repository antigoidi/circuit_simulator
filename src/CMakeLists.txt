add_executable(circuit_simulator main.cpp)

add_subdirectory(matrix)
add_subdirectory(parser)

target_link_libraries(circuit_simulator
  PRIVATE
    matrix
    parser
  )