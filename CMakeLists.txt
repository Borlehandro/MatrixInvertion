cmake_minimum_required(VERSION 3.15)
project(MatrixInvertion)

set(CMAKE_CXX_STANDARD 14)
find_package(BLAS REQUIRED)

include_directories(${BLAS_INCLUDE_DIR})

add_executable(MatrixInvertion main.cpp)
add_executable(Vector manuallyVectorisation.cpp)
add_executable(Blas blas.cpp)

target_link_libraries(Blas ${BLAS_LIBRARIES})