cmake_minimum_required(VERSION 2.8.4)
project(Aergia)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

set(SOURCE_FILES
    src/Common/Config.cpp
    src/Common/Config.h
    src/Common/Constants.h
    src/Common/GLState.cpp
    src/Common/GLState.h
    src/Algorithms/Algorithms.h
    src/Algorithms/MarchingCubes.cpp
    src/Algorithms/MarchingCubes.h
    src/Algorithms/MCTables.h
    src/Algorithms/TerrainGenerator.cpp
    src/Algorithms/TerrainGenerator.h
    src/Algorithms/FluidSimulator.cpp
    src/Algorithms/FluidSimulator.h
    src/Graphics/Helpers/Grid.cpp
    src/Graphics/Helpers/Grid.h
    src/Graphics/Helpers/Shapes.cpp
    src/Graphics/Helpers/Shapes.h
    src/Graphics/Rendering/Shader.cpp
    src/Graphics/Rendering/Shader.h
    src/Graphics/Rendering/ShaderManager.cpp
    src/Graphics/Rendering/ShaderManager.h
    src/Graphics/Scene/Camera.cpp
    src/Graphics/Scene/Camera.h
    src/Graphics/Scene/Scene.cpp
    src/Graphics/Scene/Scene.h
    src/Graphics/Scene/SceneManager.cpp
    src/Graphics/Scene/SceneManager.h
    src/Graphics/Scene/SceneObject.cpp
    src/Graphics/Scene/SceneObject.h
    src/Graphics/Scene/TMesh.h
    src/Graphics/OpenGL.cpp
    src/Graphics/OpenGL.h
    src/GUI/Elements/Button.cpp
    src/GUI/Elements/Button.h
    src/GUI/Graphics/GUIUtils.cpp
    src/GUI/Graphics/GUIUtils.h
    src/GUI/Graphics/UIGraphic.cpp
    src/GUI/Graphics/UIGraphic.h
    src/GUI/Graphics/UIShapes.cpp
    src/GUI/Graphics/UIShapes.h
    src/GUI/UI/InteractionMethod.cpp
    src/GUI/UI/InteractionMethod.h
    src/GUI/UI/Trackball.cpp
    src/GUI/UI/Trackball.h
    src/GUI/UI/TrackballInterface.cpp
    src/GUI/UI/TrackballInterface.h
    src/GUI/UI/TrackMode.cpp
    src/GUI/UI/TrackMode.h
    src/GUI/GUI.cpp
    src/GUI/GUI.h
    src/GUI/GUIManager.cpp
    src/GUI/GUIManager.h
    src/IO/Resources/FrameBuffer.cpp
    src/IO/Resources/FrameBuffer.h
    src/IO/Resources/Texture.cpp
    src/IO/Resources/Texture.h
    src/IO/Resources/TextureManager.cpp
    src/IO/Resources/TextureManager.h
    src/IO/DisplayUtils.cpp
    src/IO/DisplayUtils.h
    src/IO/GraphicsDisplay.cpp
    src/IO/GraphicsDisplay.h
    src/IO/InputMap.cpp
    src/IO/InputMap.h
    src/Math/Line.cpp
    src/Math/Line.h
    src/Math/MathUtils.cpp
    src/Math/MathUtils.h
    src/Math/Plane.h
    src/Math/Sphere.h
    src/Math/Transform.cpp
    src/Math/Transform.h
    src/Utils/DebugLogger.cpp
    src/Utils/DebugLogger.h
    src/Aergia.h)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/Algorithms)
include_directories(${PROJECT_SOURCE_DIR}/src/IO)
include_directories(${PROJECT_SOURCE_DIR}/src/IO/Resources)
include_directories(${PROJECT_SOURCE_DIR}/src/Common)
include_directories(${PROJECT_SOURCE_DIR}/src/Math)
include_directories(${PROJECT_SOURCE_DIR}/src/GUI)
include_directories(${PROJECT_SOURCE_DIR}/src/GUI/Graphics)
include_directories(${PROJECT_SOURCE_DIR}/src/GUI/UI)
include_directories(${PROJECT_SOURCE_DIR}/src/Graphics)
include_directories(${PROJECT_SOURCE_DIR}/src/Graphics/Helpers)
include_directories(${PROJECT_SOURCE_DIR}/src/Graphics/Scene)
include_directories(${PROJECT_SOURCE_DIR}/src/Graphics/Rendering)

add_library(Aergia STATIC ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

include_directories (${OPENGL_INCLUDE_DIR})
set(LIBRARIES gmp gmpxx GL GLU glut X11 Xxf86vm Xrandr pthread Xi m Xext GLEW Xmu freetype ftgl blas)

target_link_libraries(Aergia ${GLFW_LIBRARIES})
target_link_libraries(Aergia ${GLFW_STATIC_LIBRARIES})

target_link_libraries (Aergia ${LIBRARIES})
target_link_libraries(Aergia /usr/local/lib/libSOIL.a)

add_subdirectory (examples)