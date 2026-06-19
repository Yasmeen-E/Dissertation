cmake_minimum_required(VERSION 3.30)
project(OpenGLApp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)


file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw GLEW::GLEW)

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
