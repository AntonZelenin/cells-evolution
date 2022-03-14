cmake_minimum_required(VERSION 3.21)
project(cells_evolution)

set(CMAKE_CXX_STANDARD 20)

add_executable(cells_evolution src/main.cpp src/Core/App.cpp include/CellsEvo/Core/App.h src/Core/Cell.cpp include/CellsEvo/Core/Drawer.h src/Graphics/Drawer.cpp include/CellsEvo/Graphics/Drawer.h include/CellsEvo/Graphics.h include/CellsEvo/Core.h)
include_directories(include)
link_directories(lib)

file(GLOB LIBRARIES "lib/*.dylib")
target_link_libraries(cells_evolution ${LIBRARIES})