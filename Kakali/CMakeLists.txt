cmake_minimum_required(VERSION 3.6)
project(Kakali LANGUAGES CXX)

# Set output directories (bin and obj dirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Kakali)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Kakali)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR}/Kakali)

# Collect source files (recursive glob is discouraged but used here for brevity)
file(GLOB_RECURSE KAKALI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
)

add_executable(Kakali ${KAKALI_SOURCES})

target_compile_features(Kakali PRIVATE cxx_std_20)

# Include directories
target_include_directories(Kakali PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/glad/include
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/stb
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/imgui
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/glm
)

# Link dependencies
target_link_libraries(Kakali PRIVATE
    glad
    glfw
    stb
    imgui
)

# Define preprocessor macros
target_compile_definitions(Kakali PRIVATE
    GLEW_STATIC
)

# Platform-specific stuff
if(WIN32)
    target_compile_definitions(Kakali PRIVATE KR_PLATFORM_WINDOWS)
    target_link_libraries(Kakali PRIVATE opengl32)
elseif(UNIX)
    target_compile_definitions(Kakali PRIVATE GLFW_USE_X11 KR_PLATFORM_LINUX)
    target_link_libraries(Kakali PRIVATE GL pthread dl)
endif()

# Configurations flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Kakali PRIVATE _DEBUG _CONSOLE)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(Kakali PRIVATE NDEBUG _CONSOLE)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
elseif(CMAKE_BUILD_TYPE STREQUAL "Distribution")
    target_compile_definitions(Kakali PRIVATE DISTRIBUTION)
    set_property(TARGET Kakali PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()
