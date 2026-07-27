cmake_minimum_required(VERSION 3.20)

project(FPSdemo VERSION 0.1.0 LANGUAGES CXX)

add_executable(fps_client client/main.cpp)

target_compile_features(fps_client PRIVATE cxx_std_20)