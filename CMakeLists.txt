cmake_minimum_required(VERSION 3.30)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

# Your own sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# ImGui sources (core + GLFW/OpenGL3 backend)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw GLEW::GLEW)

target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLEW)

if(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(CMAKE_OSX_ARCHITECTURES "arm64")
    else()
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()

    target_link_libraries(${PROJECT_NAME}
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE GL_SILENCE_DEPRECATION)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
    else()
        list(APPEND CMAKE_PREFIX_PATH "/usr/local")
    endif()
endif()
