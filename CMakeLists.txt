cmake_minimum_required(VERSION 3.20)

project(FPSdemo VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.3
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

add_library(glad STATIC third_party/glad/src/gl.c)

target_include_directories(glad PUBLIC third_party/glad/include)

add_executable(
    fps_client
    client/main.cpp
    client/App.cpp
    client/GlfwRuntime.cpp
    client/Window.cpp
    client/ShaderProgram.cpp
    client/Camera.cpp
    client/Mesh.cpp
)

target_link_libraries(fps_client
    PRIVATE
        glfw
        glad
        OpenGL::GL
        glm::glm
)

if(APPLE)
    target_compile_definitions(fps_client PRIVATE GL_SILENCE_DEPRECATION)
endif()

if(MSVC)
    target_compile_options(fps_client PRIVATE /utf-8)
endif()

target_compile_definitions(fps_client PRIVATE GLFW_INCLUDE_NONE)

target_compile_features(fps_client PRIVATE cxx_std_20)
