cmake_minimum_required(VERSION 3.22)

project(Sylver LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_host_system_information(RESULT SYLVER_SYSTEM_ARCH QUERY OS_PLATFORM)

if(WIN32)
    set(SYLVER_OS Win32)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(SYLVER_OS Linux)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_LINUX)
elseif(UNIX AND APPLE)
    set(SYLVER_OS Mac)
    list(APPEND SYLVER_DEFINES SYLVER_PLATFORM_MAC)
else()
    message(FATAL "Unknown OS")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND SYLVER_DEFINES SYLVER_DEBUG)
endif()

set(SYLVER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${SYLVER_OS}-${SYLVER_SYSTEM_ARCH})


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

set(SHADER_PATH ${CMAKE_SOURCE_DIR}/src/Graphics/Renderer/Shaders)

set(SHADER_FILES
    ${SHADER_PATH}/Basic.frag
    ${SHADER_PATH}/Basic.vert
)

set(SHADER_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/Shaders )

find_program(GLSLANGVALIDATOR "glslangValidator")
if("${GLSLANGVALIDATOR}" STREQUAL "GLSLANGVALIDATOR-NOTFOUND")
    message(FATAL_ERROR "Could not find required program 'glslangValidator'")
endif()


foreach(FILE IN ITEMS ${SHADER_FILES})
    cmake_path(GET FILE FILENAME FILE_NAME)
    string(REPLACE "." "_" SHADER_NAME ${FILE_NAME})
    string(TOUPPER ${SHADER_NAME}_SPV SPIRV_VARIABLE_NAME)
    set(SPIRV_HEADER_FILE ${SHADER_INCLUDE_DIR}/${SHADER_NAME}_spv.h)
    add_custom_command(
        OUTPUT ${SPIRV_HEADER_FILE}
        COMMAND ${GLSLANGVALIDATOR} -V --variable-name ${SPIRV_VARIABLE_NAME} -o ${SPIRV_HEADER_FILE} ${FILE}
        MAIN_DEPENDENCY ${SOURCE_FILE}
    )
    set(SHADER_HEADERS ${SHADER_HEADERS} ${SPIRV_HEADER_FILE})

endforeach()
add_custom_target(SylverShaders
    DEPENDS ${SHADER_HEADERS}
    SOURCES ${SHADER_FILES}
)
add_dependencies(Sylver SylverShaders)

target_include_directories(Sylver 
    PRIVATE src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include
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

add_custom_command(TARGET Sylver PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Sylver>/assets)


