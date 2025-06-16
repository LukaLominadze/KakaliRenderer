cmake_minimum_required(VERSION 3.15)
project(stb)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/stb)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/stb)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/stb)

# Collect source files
set(STB_SOURCES
    stb_image.cpp
    # If you have other cpp files, add here
)

add_library(stb STATIC ${STB_SOURCES})

target_include_directories(stb PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Platform specific settings
if(WIN32)
    # Use static runtime equivalent in MSVC
    set_property(TARGET stb PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif()

# Configurations
# For single-config generators, you can do:
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(stb PRIVATE DEBUG)
    target_compile_options(stb PRIVATE /Zi)  # MSVC debug info (optional)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(stb PRIVATE NDEBUG)
    target_compile_options(stb PRIVATE /O2)  # MSVC optimization (optional)
endif()
