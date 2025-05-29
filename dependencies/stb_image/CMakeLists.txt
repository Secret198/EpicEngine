cmake_minimum_required (VERSION 3.3)
project(stb_image)
add_library(stb_image "stb_image.cpp")

target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})