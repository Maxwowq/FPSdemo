cmake_minimum_required(VERSION 3.20)

project(FPSdemo VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

add_executable(fps_client client/main.cpp client/App.cpp)

target_link_libraries(fps_client PRIVATE glfw)

target_compile_features(fps_client PRIVATE cxx_std_20)