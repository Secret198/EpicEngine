cmake_minimum_required (VERSION 3.3)
project(glad)
add_library(glad "src/glad.c")

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)