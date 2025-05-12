cmake_minimum_required(VERSION 3.22)

find_package(fmt CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)



list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)


file(GLOB_RECURSE SYLVER_RUNTIME_PUBLIC_HEADERS ${CMAKE_CURRENT_LIST_DIR}/inclue/**.hpp)
file(GLOB_RECURSE SYLVER_RUNTIME_PRIVATE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/**.hpp)
file(GLOB_RECURSE SYLVER_RUNTIME_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.cpp)

add_library(SylverRuntime SHARED
    ${SYLVER_RUNTIME_PUBLIC_HEADERS}
    ${SYLVER_RUNTIME_PRIVATE_HEADERS}
    ${SYLVER_RUNTIME_SOURCES}
)

set_target_properties(SylverRuntime PROPERTIES VERSION 0.1.0)

target_compile_definitions(SylverRuntime
    PUBLIC ${SYLVER_DEFINES}
    PRIVATE SYLVER_DLL_EXPORT
)
# Shader Compilation
include(Shaders)

add_dependencies(SylverRuntime SylverShaders)

target_include_directories(SylverRuntime 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE ${Stb_INCLUDE_DIR}
)

target_precompile_headers(SylverRuntime
    PRIVATE ${CMAKE_SOURCE_DIR}/SylverCommon/include/Defines.hpp
)

target_link_libraries(SylverRuntime 
    PUBLIC SylverCommon
    PRIVATE fmt::fmt
    PRIVATE spdlog::spdlog
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE glfw
    PRIVATE Vulkan::Vulkan
    PRIVATE glad::glad
    PRIVATE OpenGL::GL
    PRIVATE EnTT::EnTT
    PRIVATE imgui::imgui
)

add_custom_command(TARGET SylverRuntime PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/assets ${SYLVER_OUTPUT_DIR}/assets)


