cmake_minimum_required(VERSION 3.22)

project(Sylver LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_host_system_information(RESULT SYLVER_ARCH QUERY OS_PLATFORM)

if(WIN32)
    set(SYLVER_OS Win32)
    set(SYLVER_OS_DEFINE SYLVER_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(SYLVER_OS Linux)
    set(SYLVER_OS_DEFINE SYLVER_PLATFORM_LINUX)
elseif(UNIX AND APPLE)
    set(SYLVER_OS Mac)
    set(SYLVER_OS_DEFINE SYLVER_PLATFORM_MAC)
else()
    message(FATAL "Unknown OS")
endif()

set(SYLVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/out/${SYLVER_OS}/${SYLVER_ARCH})
message(${SYLVER_OUTPUT_DIR})



find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SYLVER_OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SYLVER_OUTPUT_DIR}/lib)
set(PROJECT_VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Sylver)



target_compile_definitions(Sylver PUBLIC
    ${SYLVER_OS_DEFINE}
)
if(SYLVER_USE_OPENGL)
    message("Using OpenGL")
    target_compile_definitions(Sylver PRIVATE SYLVER_USE_OPENGL)
endif()

target_include_directories(Sylver 
    PUBLIC include
    PRIVATE src
)

target_link_libraries(Sylver PRIVATE 
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    glfw
    Vulkan::Vulkan
    glad::glad
    OpenGL::GL
)
add_subdirectory(src)
