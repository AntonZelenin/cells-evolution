cmake_minimum_required(VERSION 3.21)
project(cells_evolution)

set(CMAKE_CXX_STANDARD 20)

add_executable(cells_evolution src/main.cpp src/app.cpp include/CellsEvo/app.h src/cell.cpp include/CellsEvo/cell.h src/graphics/drawer.cpp include/CellsEvo/graphics/drawer.h include/CellsEvo/graphics.h src/food.cpp include/CellsEvo/food.h include/CellsEvo/geometry.h src/world.cpp include/CellsEvo/world.h include/CellsEvo/base.h src/tools.cpp include/CellsEvo/tools.h include/CellsEvo/logic/logic.h src/logic/logic.cpp src/genetics/genes.cpp include/CellsEvo/genetics/genes.h src/geometry.cpp src/logic/cell_logic.cpp include/CellsEvo/logic/cell_logic.h src/genetics/engineer.cpp include/CellsEvo/genetics/engineer.h src/cell_generator.cpp include/CellsEvo/cell_generator.h src/collision_resolution.cpp include/CellsEvo/collision_resolution.h)
include_directories(include)
link_directories(lib)

file(GLOB LIBRARIES "lib/*.dylib")
target_link_libraries(cells_evolution ${LIBRARIES})