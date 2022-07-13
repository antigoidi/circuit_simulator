add_library(matrix "")

target_sources(matrix
  PRIVATE
    BJT.cpp
    diode.cpp
    matrix.cpp
    MOSFET.cpp
    nonlinear.cpp
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/BJT.h
    ${CMAKE_CURRENT_LIST_DIR}/device.h
    ${CMAKE_CURRENT_LIST_DIR}/diode.h
    ${CMAKE_CURRENT_LIST_DIR}/matrix.h
    ${CMAKE_CURRENT_LIST_DIR}/MOSFET.h
    ${CMAKE_CURRENT_LIST_DIR}/nonlinear.h
  )

target_include_directories(matrix
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

target_link_libraries(matrix
  PRIVATE
    parser
    Eigen3::Eigen
  )