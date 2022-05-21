cmake_minimum_required(VERSION 3.21)
project(cells_evolution)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        cells_evolution
        src/main.cpp
        src/app/app.cpp
        include/CellsEvo/app/app.h
        src/core/cell.cpp
        include/CellsEvo/core/cell.h
        src/graphics/drawer.cpp
        include/CellsEvo/graphics/drawer.h
        include/CellsEvo/graphics.h
        src/core/food.cpp
        include/CellsEvo/core/food.h
        src/core/geometry.cpp
        include/CellsEvo/core/geometry.h
        src/core/world.cpp
        include/CellsEvo/core/world.h
        include/CellsEvo/core/base.h
        src/core/tools.cpp
        include/CellsEvo/core/tools.h
        src/logic/logic.cpp
        include/CellsEvo/logic/logic.h
        src/genetics/genes.cpp
        include/CellsEvo/genetics/genes.h
        src/logic/cell_logic.cpp
        include/CellsEvo/logic/cell_logic.h
        src/genetics/engineer.cpp
        include/CellsEvo/genetics/engineer.h
        src/core/cell_generator.cpp
        include/CellsEvo/core/cell_generator.h
        src/core/collision_resolution.cpp
        include/CellsEvo/core/collision_resolution.h
        src/graphics/camera.cpp
        include/CellsEvo/graphics/camera.h
        include/imgui/imgui.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui-SFML.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_widgets.cpp
        src/app/gui.cpp include/CellsEvo/app/gui.h include/CellsEvo/core/types.h src/core/events.cpp include/CellsEvo/core/events.h src/core/food_generator.cpp include/CellsEvo/core/food_generator.h)
include_directories(include)
link_directories(lib)

file(GLOB
        LIBRARIES "lib/*.dylib")
target_link_libraries(cells_evolution ${LIBRARIES})

# this was needed for imgui_impl_metal.cpp
#find_library(CoreFoundation_Library CoreFoundation)
#mark_as_advanced(CoreFoundation_Library)
#target_link_libraries(cells_evolution ${CoreFoundation_Library})

find_library(OpenGL_Library OpenGL)
target_link_libraries(cells_evolution ${OpenGL_Library})