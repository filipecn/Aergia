cmake_minimum_required(VERSION 2.8.4)
project(ProceduralTerrains)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/bin/ProceduralTerrains")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/atlas DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(proceduralTerrains proceduralTerrains.cpp)
target_link_libraries(proceduralTerrains LINK_PUBLIC Aergia)