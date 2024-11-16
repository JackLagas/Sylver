cmake_minimum_required(VERSION 3.22)

target_sources(Sylver PRIVATE 
    Window.cpp
    Window.hpp
    Event.hpp
    Renderer.cpp
    Renderer.hpp
)

add_subdirectory(Window/Platform/GLFW)
add_subdirectory(Renderer)
