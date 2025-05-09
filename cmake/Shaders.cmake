set(OPENGL_SHADER_PATH ${CMAKE_SOURCE_DIR}/src/Graphics/Renderer/Platform/OpenGL/Shaders)
set(VULKAN_SHADER_PATH ${CMAKE_SOURCE_DIR}/src/Graphics/Renderer/Platform/Vulkan/Shaders)

file(GLOB_RECURSE OPENGL_SHADER_FILES ${OPENGL_SHADER_PATH}/*.frag ${OPENGL_SHADER_PATH}/*.vert)
file(GLOB_RECURSE VULKAN_SHADER_FILES ${VULKAN_SHADER_PATH}/*.frag ${VULKAN_SHADER_PATH}/*.vert)

set(SHADER_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include/Shaders )

find_program(GLSLANGVALIDATOR "glslangValidator")
if("${GLSLANGVALIDATOR}" STREQUAL "GLSLANGVALIDATOR-NOTFOUND")
    message(FATAL_ERROR "Could not find required program 'glslangValidator'")
endif()


foreach(SOURCE_FILE IN ITEMS ${VULKAN_SHADER_FILES})
    cmake_path(GET SOURCE_FILE FILENAME FILE_NAME)
    string(REPLACE "." "_" SHADER_NAME ${FILE_NAME})
    string(TOUPPER ${SHADER_NAME}_SPV SPIRV_VARIABLE_NAME)
    set(SPIRV_HEADER_FILE ${SHADER_INCLUDE_DIR}/Vulkan/${SHADER_NAME}_spv.h)
    add_custom_command(
        OUTPUT ${SPIRV_HEADER_FILE}
        COMMAND ${GLSLANGVALIDATOR} -V --variable-name ${SPIRV_VARIABLE_NAME} -o ${SPIRV_HEADER_FILE} ${SOURCE_FILE}
        MAIN_DEPENDENCY ${SOURCE_FILE}
    )
    set(SHADER_HEADERS ${SHADER_HEADERS} ${SPIRV_HEADER_FILE})

endforeach()
foreach(SOURCE_FILE IN ITEMS ${OPENGL_SHADER_FILES})
    cmake_path(GET SOURCE_FILE FILENAME FILE_NAME)
    string(REPLACE "." "_" SHADER_NAME ${FILE_NAME})
    set(HEADER_FILE ${SHADER_INCLUDE_DIR}/OpenGL/${SHADER_NAME}.h)
    add_custom_command(
        OUTPUT ${HEADER_FILE}
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/ShaderHeader.cmake ${SOURCE_FILE} ${HEADER_FILE}
        MAIN_DEPENDENCY ${SOURCE_FILE}
    )
    set(SHADER_HEADERS ${SHADER_HEADERS} ${HEADER_FILE})


endforeach()
add_custom_target(SylverShaders
    DEPENDS ${SHADER_HEADERS}
    SOURCES ${SHADER_FILES}
)
