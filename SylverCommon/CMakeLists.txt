cmake_minimum_required(VERSION 3.22)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB_RECURSE SYLVER_COMMON_PUBLIC_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/**.hpp)
file(GLOB_RECURSE SYLVER_COMMON_PRIVATE_HEADERS ${CMAKE_CURRENT_LIST_DIR}/src/**.hpp)
file(GLOB_RECURSE SYLVER_COMMON_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/**.cpp)

add_library(SylverCommon STATIC
    ${SYLVER_COMMON_PUBLIC_HEADERS}
    ${SYLVER_COMMON_PRIVATE_HEADERS}
    ${SYLVER_COMMON_SOURCES}
)
set_target_properties(SylverCommon PROPERTIES VERSION 0.1.0)

target_compile_definitions(SylverCommon PUBLIC
    ${SYLVER_DEFINES}
)

target_include_directories(SylverCommon 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
    PUBLIC ${CMAKE_BINARY_DIR}/include
)

target_precompile_headers(SylverCommon
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/Defines.hpp
)

target_link_libraries(SylverCommon
    PUBLIC fmt::fmt
    PUBLIC spdlog::spdlog
    PUBLIC nlohmann_json::nlohmann_json
)



