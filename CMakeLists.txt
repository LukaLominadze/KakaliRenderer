cmake_minimum_required(VERSION 3.6)

project(KakaliRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Distribution" CACHE STRING "" FORCE)

# Output directory format similar to your premake outputdir
set(OUTPUT_DIR ${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

# Add dependencies (glfw, stb, imgui are assumed to have their own CMakeLists)
add_subdirectory(vendor/dependencies/glfw)
add_subdirectory(vendor/dependencies/stb)
add_subdirectory(vendor/dependencies/imgui)
add_subdirectory(vendor/dependencies/glad)

# Add Kakali app
add_subdirectory(Kakali)
