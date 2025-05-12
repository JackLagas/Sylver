cmake_minimum_required(VERSION 3.22)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)


file(GLOB_RECURSE SYLVER_ENGINE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/**.hpp)
file(GLOB_RECURSE SYLVER_ENGINE_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.cpp)

add_executable(SylverEngine
    ${SYLVER_ENGINE_HEADERS}
    ${SYLVER_ENGINE_SOURCES}
)
set_target_properties(SylverEngine PROPERTIES VERSION 0.1.0)

target_compile_definitions(SylverEngine PUBLIC
    ${SYLVER_DEFINES}
)

target_include_directories(SylverEngine 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)

target_precompile_headers(SylverEngine
    PRIVATE ${CMAKE_SOURCE_DIR}/SylverCommon/include/Defines.hpp
)

target_link_libraries(SylverEngine PRIVATE 
    PUBLIC SylverCommon
    PUBLIC SylverRuntime
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    glfw
    Vulkan::Vulkan
    glad::glad
    OpenGL::GL
    imgui::imgui
)

