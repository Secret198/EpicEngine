# CMakeList.txt : CMake project for EpicEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.3)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


project ("EpicEngine")
# Add source to this project's executable.
add_executable (EpicEngine "EpicEngine.cpp" "EpicEngine.h" "camera.h" "shaderClass.h" "model.h" "mesh.h" "inputHandler.h" "imGuiHandler.h" "Lights/Light.h" "Lights/PointLight.h" "Lights/DirectionalLight.h" "Lights/SpotLight.h" "MarchingCubes.h" "PerlinNoise.h" "Object.h" "PostProcessing.h" "Lights/PointLight.cpp" "Lights/DirectionalLight.cpp" "Lights/SpotLight.cpp" "imGuiHandler.cpp" "inputHandler.cpp" "camera.cpp" "MarchingCubes.cpp" "mesh.cpp" "model.cpp" "PerlinNoise.cpp" "PostProcessing.cpp" )


add_subdirectory(dependencies/stb_image)
add_subdirectory(dependencies/imgui)
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glm-1.0.1)

target_include_directories(EpicEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/includes)
target_link_libraries(EpicEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/glfw3.lib)

target_include_directories(EpicEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp/includes")
target_link_libraries(EpicEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp/assimp-vc143-mtd.lib")
#target_link_libraries(EpicEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp/assimp-vc143-mtd.exp)
#target_link_libraries(EpicEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp/assimp-vc143-mtd.dll")
target_link_directories(EpicEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp")
add_custom_command(TARGET EpicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/assimp/assimp-vc143-mtd.dll"
    $<TARGET_FILE_DIR:EpicEngine>)

add_custom_command(TARGET EpicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/models"
    $<TARGET_FILE_DIR:EpicEngine>/models)

add_custom_command(TARGET EpicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/textures"
    $<TARGET_FILE_DIR:EpicEngine>/textures)

add_custom_command(TARGET EpicEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
    $<TARGET_FILE_DIR:EpicEngine>/shaders)

file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")

foreach(SHADER ${SHADERS})
    configure_file("${SHADER}" ${CMAKE_BINARY_DIR}/shaders COPYONLY)

#    add_custom_command(TARGET EpicEngine POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy "${SHADER}"
#        $<TARGET_FILE_DIR:EpicEngine>/shaders)
endforeach()



target_link_libraries(EpicEngine PUBLIC glad)
target_link_libraries(EpicEngine PUBLIC imgui)
target_link_libraries(EpicEngine PUBLIC stb_image)
target_link_libraries(EpicEngine PUBLIC glm)


target_compile_definitions(EpicEngine
  PRIVATE
    $<$<CONFIG:Release>:RELEASE_BUILD>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EpicEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
