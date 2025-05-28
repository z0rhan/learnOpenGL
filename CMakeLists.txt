cmake_minimum_required(VERSION 3.31)
project(learnOpenGl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

find_package(OpenGL REQUIRED)

set(SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/VertexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/IndexBuffer.cpp
    ${CMAKE_SOURCE_DIR}/src/VertexArray.cpp
    ${CMAKE_SOURCE_DIR}/src/VertexBufferLayout.cpp
    ${CMAKE_SOURCE_DIR}/src/error.cpp
    ${CMAKE_SOURCE_DIR}/src/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_image/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/glad/src/glad.c
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/vendor
)

target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)

# Tests
set(TEST_FILES
    ${CMAKE_SOURCE_DIR}/test/main.cpp
    ${CMAKE_SOURCE_DIR}/glad/src/glad.c
    ${CMAKE_SOURCE_DIR}/src/vendor/stb_image/stb_image.cpp
)

add_executable(${PROJECT_NAME}_test ${TEST_FILES})

target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/vendor
)

target_link_libraries(${PROJECT_NAME}_test
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
)
