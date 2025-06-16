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
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/assimp/include
)

message(STATUS "Assimp lib path: ${CMAKE_SOURCE_DIR}/vendor/dependencies/assimp/Debug/assimp-vc143-mtd.lib")

# Link dependencies
target_link_libraries(Kakali PRIVATE
    glad
    glfw
    stb
    imgui
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/assimp/Debug/assimp-vc143-mtd.lib
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/assimp/Debug/unit.lib
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

# Post build: copy assimp dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/vendor/dependencies/assimp/assimp-vc143-mtd.dll
    $<TARGET_FILE_DIR:${PROJECT_NAME}>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Kakali/src/res/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Kakali/src/res/
    ${CMAKE_CURRENT_BINARY_DIR}/src/res
)