cmake_minimum_required(VERSION 3.25)

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

set(PROJECT_NAME_STR "tiny_renderer_study")
project(${PROJECT_NAME_STR} VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# BUILD TYPE DEFINITION
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Build type (Debug or Release)" FORCE)
endif()

# ==============================================================================
# COMPILER AND FLAGS
# ==============================================================================

if(MSVC)
    add_compile_options(/W4 /WX /Zc:__cplusplus)
    add_compile_options(/MP /diagnostics:column /FC /Zc:inline)
else()
    add_compile_options(-Wall -Wextra
                        -Wno-error=ignored-attributes)
endif()

# ==============================================================================
# EXTERNAL DEPENDENCIES (FetchContent)
# ==============================================================================



# ==============================================================================
# Download de Headers Single-File (STB)
# ==============================================================================



# ==============================================================================
# TARGET CONFIGURATION
# ==============================================================================

file(GLOB_RECURSE SOURCES "src/*.cpp")

if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in src/ !")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

# ==============================================================================
# INCLUDES AND LINKING 
# ==============================================================================

target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src" 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/"
)
