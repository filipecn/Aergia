cmake_minimum_required(VERSION 2.8.4)
project(FluidSimulation2D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/bin/FluidSimulation2D")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(fluidSimulation2D fluidSimulation2D.cpp)
target_link_libraries(fluidSimulation2D LINK_PUBLIC Aergia)