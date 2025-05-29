cmake_minimum_required (VERSION 3.3)

project(imgui)
add_library(imgui imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_impl_glfw.cpp imgui_impl_opengl3.cpp imgui_stdlib.cpp imgui_tables.cpp imgui_widgets.cpp)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

get_filename_component(PARENT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

target_include_directories(imgui PUBLIC ${PARENT_DIRECTORY}/glfw/includes)
target_link_libraries(imgui PRIVATE ${PARENT_DIRECTORY}/glfw/glfw3.lib)