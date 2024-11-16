cmake_minimum_required(VERSION 3.22)

target_sources(Sylver PRIVATE 
    VulkanRenderer.cpp
    VulkanRenderer.hpp
    VulkanContext.cpp
    VulkanContext.hpp
    VulkanDebugMessenger.hpp
    VulkanDebugMessenger.cpp
    VulkanSwapchain.cpp
    VulkanSwapchain.hpp
)
