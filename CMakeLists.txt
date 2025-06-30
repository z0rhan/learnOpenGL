cmake_minimum_required(VERSION 3.31)
project(learnOpenGl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

find_package(OpenGL REQUIRED)

# Getting started
set(GETTING_STARTED
    ${CMAKE_SOURCE_DIR}/src/gettingStarted/main.cpp
    ${CMAKE_SOURCE_DIR}/src/gettingStarted/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/gettingStarted/error.cpp
    ${CMAKE_SOURCE_DIR}/src/gettingStarted/Camera.cpp
    ${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)

add_executable(getting_started ${GETTING_STARTED})

target_include_directories(getting_started PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor
)

# Link libraries conditionally based on the platform
if(WIN32)
    target_link_libraries(getting_started
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/glfw3.lib   # Use .lib on Windows
    )
else()
    target_link_libraries(getting_started
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/libglfw3.a     # Use .a on Linux
    )
endif()

# lighting
set(LIGHTING
    ${CMAKE_SOURCE_DIR}/src/lighting/main.cpp
    ${CMAKE_SOURCE_DIR}/src/lighting/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/lighting/error.cpp
    ${CMAKE_SOURCE_DIR}/src/lighting/Camera.cpp
    ${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)

add_executable(lighting ${LIGHTING})

target_include_directories(lighting PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor
)

# Link libraries conditionally based on the platform
if(WIN32)
    target_link_libraries(lighting
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/glfw3.lib   # Use .lib on Windows
    )
else()
    target_link_libraries(lighting
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/libglfw3.a     # Use .a on Linux
    )
endif()

# model loading
set(MODEL
    ${CMAKE_SOURCE_DIR}/src/modelLoading/main.cpp
    ${CMAKE_SOURCE_DIR}/src/modelLoading/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/modelLoading/error.cpp
    ${CMAKE_SOURCE_DIR}/src/modelLoading/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/modelLoading/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/modelLoading/Model.cpp
    ${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)

add_executable(model ${MODEL})

target_include_directories(model PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
)

# Link libraries conditionally based on the platform
if(WIN32)
    target_link_libraries(model
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/glfw3.lib   # Use .lib on Windows
        ${CMAKE_SOURCE_DIR}/vendor/assimp/lib/assimp-vc143-mtd.lib  # Use .lib on Windows
        # dependencies for assimp on Windows
        zlibstatic
        minizipstatic
    )
else()
    target_link_libraries(model
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/vendor/GLFW/lib/libglfw3.a     # Use .a on Linux
        ${CMAKE_SOURCE_DIR}/vendor/assimp/lib/libassimp.a    # Use .a on Linux
        # dependencies for assimp on Linux
        z
        minizip
    )
endif()