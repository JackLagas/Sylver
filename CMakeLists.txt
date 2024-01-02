cmake_minimum_required(VERSION 3.22)


project(Sylver VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
set(BUILD_SHARED_LIBS ON)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)



option(BUILD_EXECUTABLE "build and link test executable" OFF)
option(VERBOSE_LOGGING "Enable verbose logs" OFF)

set(SOURCES 
    src/Logger.cpp
    src/entry.cpp
    src/Level.cpp
    src/Vector.cpp
    src/Entity.cpp
    src/Sprite.cpp
    src/Sylver.cpp
)
set(SYLVER_API_HEADERS
    include/Sylver.hpp
)


add_library(Sylver SHARED ${SOURCES})
target_link_libraries(Sylver PRIVATE raylib)


set_target_properties(Sylver PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADERS ${SYLVER_API_HEADERS}
)
target_include_directories(Sylver PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Sylver PRIVATE ${CMAKE_SOURCE_DIR}/src)



if(BUILD_EXECUTABLE)

    add_executable(TestApp ${CMAKE_SOURCE_DIR}/Sandbox/main.cpp)
    target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(TestApp Sylver)
    
endif()

