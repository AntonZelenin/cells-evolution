cmake_minimum_required(VERSION 3.21)
project(cells_evolution)

set(CMAKE_CXX_STANDARD 20)

add_executable(cells_evolution main.cpp)
include_directories(include)
link_directories(lib)

file(GLOB LIBRARIES "lib/*.dylib")
target_link_libraries(cells_evolution ${LIBRARIES})