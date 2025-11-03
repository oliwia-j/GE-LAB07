project(pacman)
cmake_minimum_required(VERSION 3.11)
# Require modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#### Setup Directories ####
#Main output directory
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/SFML")
set(SFML_INCS "lib/SFML/include")
link_directories("${CMAKE_BINARY_DIR}/lib/SFML/lib")


#### level system ####
add_library(tile_level STATIC tile_level_loader/level_system.cpp)
target_include_directories(tile_level INTERFACE tile_level_loader)
target_link_libraries(tile_level sfml-graphics)

#### Engine ####
add_library(engine STATIC engine/game_system.cpp 
                        engine/renderer.cpp 
                        engine/ecm.cpp)
target_include_directories(engine INTERFACE egine)
target_link_libraries(engine sfml-graphics)

#### executable ####
add_executable(pacman 
    main.cpp 
    components.cpp
    scenes.cpp)
target_include_directories(pacman PRIVATE ${SFML_INCS} tile_level_loader engine)
target_link_libraries(pacman sfml-graphics tile_level engine)
set_target_properties(pacman 
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
)
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} 
  -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
)
add_dependencies(pacman copy_resources)