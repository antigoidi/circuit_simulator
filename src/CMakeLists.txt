add_executable(circuit_simulator main.cpp)

add_subdirectory(netlist)
add_subdirectory(parser)

target_link_libraries(circuit_simulator
  PRIVATE
    #netlist
    parser
  )