cmake_minimum_required(VERSION 3.22.1)
project("vulkan_tile_backend")

# Buscar la librería de Vulkan nativa del Android NDK
find_library(vulkan-lib vulkan REQUIRED)
find_library(log-lib log REQUIRED)

# Buscar el compilador de shaders glslc
find_program(GLSLC_EXECUTABLE glslc HINTS ${ANDROID_NDK}/shader-tools/linux-x86_64)

set(SHADER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/shaders/tile_processor.comp)
set(SHADER_BINARY ${CMAKE_CURRENT_BINARY_DIR}/shaders/tile_processor.spv)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

if(GLSLC_EXECUTABLE)
    add_custom_command(
        OUTPUT ${SHADER_BINARY}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=compute ${SHADER_SOURCE} -o ${SHADER_BINARY}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compilando GLSL a SPIR-V: ${SHADER_SOURCE}"
    )
    add_custom_target(CompileShaders DEPENDS ${SHADER_BINARY})
endif()

add_library(
    vulkan_tile_backend
    SHARED
    vulkan_tile_backend.cpp
)

if(GLSLC_EXECUTABLE)
    add_dependencies(vulkan_tile_backend CompileShaders)
endif()

target_include_directories(vulkan_tile_backend PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(
    vulkan_tile_backend
    ${vulkan-lib}
    ${log-lib}
)
