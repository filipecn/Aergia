cmake_minimum_required(VERSION 2.8.4)
project(Simple3D)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}/bin/Simple3D")

add_executable(simple3D simple3D.cpp)
target_link_libraries(simple3D LINK_PUBLIC Aergia)