cmake_minimum_required(VERSION 2.8)
project(OpenGLProgram)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
include_directories(/usr/local/include/freetype2)
include_directories(/System/Library/Frameworks)
include_directories(headers)
find_library(COCOA Cocoa)
find_library(IOKIT IOKit)
find_library(OPENGL OpenGL)
find_library(COREVIDEO CoreVideo)

set(SOURCE_FILES src/terrain.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${COCOA} ${IOKIT} ${OPENGL} ${COREVIDEO})
target_link_libraries(${PROJECT_NAME} assimp glfw3 freetype)

