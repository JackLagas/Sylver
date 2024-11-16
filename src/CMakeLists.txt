cmake_minimum_required(VERSION 3.22)

target_sources(Sylver PRIVATE Entry.cpp)

add_subdirectory(Graphics)
add_subdirectory(Core)
add_subdirectory(Modules)
