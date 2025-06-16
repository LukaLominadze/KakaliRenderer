cmake_minimum_required(VERSION 3.15)
project(imgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/libs/imgui)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/libs/imgui)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dependencies/libs/imgui)

# List source files explicitly
set(IMGUI_SOURCES
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    imgui_tables.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)

set(IMGUI_HEADERS
    imconfig.h
    imgui.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_impl_glfw.h
    imgui_impl_opengl3.h
    imgui_impl_opengl3_loader.h
)

add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_HEADERS})

# Include directories
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/glfw/include
)

# Platform specific settings
if(WIN32)
    set_property(TARGET imgui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

# Configuration-specific compile options / definitions
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(imgui PRIVATE DEBUG)
    target_compile_options(imgui PRIVATE /Zi)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(imgui PRIVATE NDEBUG)
    target_compile_options(imgui PRIVATE /O2)
endif()
