cmake_minimum_required(VERSION 3.22)

project(Sylver LANGUAGES C CXX)

set(PROJECT_VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_host_system_information(RESULT SYLVER_SYSTEM_ARCH QUERY OS_PLATFORM)

if(WIN32)
    set(SYLVER_OS Win32)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(SYLVER_OS Linux)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_LINUX)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_UNIX)
elseif(UNIX AND APPLE)
    set(SYLVER_OS Mac)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_MAC)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_UNIX)
else()
    message(FATAL "Unknown OS")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND SYLVER_DEFINES SYLVER_DEBUG)
endif()

set(SYLVER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${SYLVER_OS}-${SYLVER_SYSTEM_ARCH})


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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SYLVER_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SYLVER_OUTPUT_DIR}/lib)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


file(GLOB_RECURSE SYLVER_HEADERS ${CMAKE_SOURCE_DIR}/src/**.hpp)
file(GLOB_RECURSE SYLVER_SOURCES ${CMAKE_SOURCE_DIR}/src/**.cpp)

add_executable(Sylver
    ${SYLVER_HEADERS}
    ${SYLVER_SOURCES}
)

target_compile_definitions(Sylver PUBLIC
    ${SYLVER_DEFINES}
)
if(SYLVER_USE_OPENGL)
    message("Using OpenGL")
    target_compile_definitions(Sylver PRIVATE SYLVER_USE_OPENGL)
endif()


# Shader Compilation
include(Shaders)

add_dependencies(Sylver SylverShaders)

target_include_directories(Sylver 
    PRIVATE src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
    PRIVATE ${Stb_INCLUDE_DIR}
)

target_precompile_headers(Sylver
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Defines.hpp
)

target_link_libraries(Sylver PRIVATE 
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    glfw
    Vulkan::Vulkan
    glad::glad
    OpenGL::GL
    EnTT::EnTT
    imgui::imgui
)

add_custom_command(TARGET Sylver PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Sylver>/assets)


