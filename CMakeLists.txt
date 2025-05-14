cmake_minimum_required(VERSION 3.31)
project(learnOpenGl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-build-debug/bin)

find_package(OpenGL REQUIRED)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/glad/src/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)
