cmake_minimum_required(VERSION 3.10)

project(Craft)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory(libs/glfw)

include_directories(libs/glm libs/glad/include include libs/picopng/ libs/chaiscript)

if(MSVC)
    add_compile_options(/bigobj)
endif()

set(SRC_GLAD libs/glad/src/glad.c)
file(GLOB SRC_ENGINE
    src/Engine/*.cpp
    src/Engine/**/*.cpp
    src/Engine/**/**/*.cpp
    src/Engine/**/**/**/*.cpp)

file(GLOB SRC_GAME
    src/Game/*.cpp
    src/Game/**/*.cpp
    src/Game/**/**/*.cpp
    src/Game/**/**/**/*.cpp)

file(GLOB SRC_TETRIS_C
    src/Tetris_C/*.cpp
    src/Tetris_C/**/*.cpp
    src/Tetris_C/**/**/*.cpp
    src/Tetris_C/**/**/**/*.cpp)

file(GLOB SRC_TETRIS_S
    src/Tetris_S/*.cpp
    src/Tetris_S/**/*.cpp
    src/Tetris_S/**/**/*.cpp
    src/Tetris_S/**/**/**/*.cpp)

add_executable(Tetris_C ${SRC_TETRIS_C} ${SRC_ENGINE} ${SRC_GLAD} )
target_link_libraries(Tetris_C ${OPENGL_LIBRARIES} glfw)

add_executable(Tetris_S ${SRC_TETRIS_S} ${SRC_ENGINE} ${SRC_GLAD} )
target_link_libraries(Tetris_S ${OPENGL_LIBRARIES} glfw)

# add_executable(Dumb ${SRC_GAME} ${SRC_ENGINE} ${SRC_GLAD} )
# target_link_libraries(Dumb ${OPENGL_LIBRARIES} glfw)