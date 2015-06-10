cmake_minimum_required(VERSION 2.8.4)
project(FluidSimulation2D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/bin/FluidSimulation2D")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(SOURCE_FILES
ProceduralTexture.h
ProceduralTexture.cpp
MACGrid.h
MACGrid.cpp
SmokeSimulator.h
SmokeSimulator.cpp
fluidSimulation2D.cpp
)

add_executable(fluidSimulation2D ${SOURCE_FILES})
target_link_libraries(fluidSimulation2D LINK_PUBLIC Aergia)