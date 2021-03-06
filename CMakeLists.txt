CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(Maze)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Modules/")

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLFW3 REQUIRED)
MESSAGE(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

SET(MAZE_SOURCES
    Source/Maze/main.cpp
	Source/Maze/Object.cpp
	Source/Maze/Object.h

	Source/Maze/Shader.cpp
	Source/Maze/Shader.h

	Source/Maze/glad.c

	Source/Maze/ResourcesManager.h
	Source/Maze/ResourcesManager.cpp

	Source/Maze/Shaders/frag.fs
	Source/Maze/Shaders/frag_blend.fs
	Source/Maze/Shaders/vert.vs

	Source/Maze/SceneManager/SceneManager.cpp
	Source/Maze/SceneManager/SceneManager.h

	Source/Maze/FPSController/FPSController.h
	Source/Maze/FPSController/FPSController.cpp

	Source/Maze/Camera/Camera.cpp
	Source/Maze/Camera/Camera.h
	Source/Maze/Camera/FPSCamera.cpp
	Source/Maze/Camera/FPSCamera.h

	Source/Maze/MapLoader/MapLoader.h
	Source/Maze/MapLoader/MapLoader.cpp

	Source/Maze/ObjLoader/ObjLoader.h
	Source/Maze/ObjLoader/ObjLoader.cpp
)

SET(FRAMEWORKS)
IF(APPLE)
    FIND_LIBRARY(CARBON_LIB Carbon)
    FIND_LIBRARY(COCOA_LIB Cocoa)
    FIND_LIBRARY(IOKIT_LIB IOKit)
    FIND_LIBRARY(COREFOUNDATION_LIB CoreFoundation)

SET(FRAMEWORKS
    ${CARBON_LIB}
    ${COCOA_LIB}
    ${IOKIT_LIB}
    ${COREFOUNDATION_LIB}
    )
ENDIF (APPLE)

LINK_DIRECTORIES(Lib)

ADD_EXECUTABLE(Maze ${MAZE_SOURCES})

TARGET_INCLUDE_DIRECTORIES(Maze PRIVATE Source Include)

TARGET_LINK_LIBRARIES(Maze glfw3 ${OPENGL_gl_LIBRARY} ${FRAMEWORKS})

IF (WIN32)
SET_TARGET_PROPERTIES(Maze PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
ENDIF (WIN32)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)