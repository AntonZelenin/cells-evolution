cmake_minimum_required(VERSION 3.21)
project(cells_evolution)

set(CMAKE_CXX_STANDARD 20)

add_executable(cells_evolution src/main.cpp src/core/app.cpp include/CellsEvo/core/app.h src/core/cell.cpp include/CellsEvo/core/cell.h src/graphics/drawer.cpp include/CellsEvo/graphics/drawer.h include/CellsEvo/graphics.h include/CellsEvo/core.h src/core/food.cpp include/CellsEvo/core/food.h include/CellsEvo/core/geometry.h src/core/world.cpp include/CellsEvo/core/world.h include/CellsEvo/core/base.h src/core/tools.cpp include/CellsEvo/core/tools.h include/CellsEvo/core/logic.h src/core/logic.cpp src/core/genes.cpp include/CellsEvo/core/genes.h src/core/geometry.cpp)
include_directories(include)
link_directories(lib)

file(GLOB LIBRARIES "lib/*.dylib")
target_link_libraries(cells_evolution ${LIBRARIES})