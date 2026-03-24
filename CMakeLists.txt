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

# BUILD TYPE DEFINITIONS
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Build type (Debug or Release)" FORCE)
endif()

string(TOUPPER "${CMAKE_CXX_COMPILER_ID}" COMPILER_ID_UPPER)

if(CMAKE_CONFIGURATION_TYPES)
    # Multi-config (ex.: Visual Studio)
    set(BUILD_SOURCE "${CMAKE_SOURCE_DIR}/install/${COMPILER_ID_UPPER}/$<CONFIG>")
else()
    # Single-config (ex.: MinGW Makefiles, Ninja)
    set(BUILD_SOURCE "${CMAKE_SOURCE_DIR}/install/${COMPILER_ID_UPPER}/${CMAKE_BUILD_TYPE}")
endif()

set(CMAKE_INSTALL_PREFIX ${BUILD_SOURCE} CACHE PATH "Diretório de instalação")

# ==============================================================================
# EXTERNAL DEPENDENCIES (FetchContent)
# ==============================================================================

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

# Google Benchmark FetchContent - Start
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON CACHE BOOL "" FORCE)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libs" FORCE)

FetchContent_Declare(
  google_benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.5
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND MINGW
   AND CMAKE_CXX_COMPILER MATCHES ".*/clang64/bin/clang\\+\\+(.exe)?$")
  set(HAVE_STD_REGEX ON CACHE BOOL
      "Force std::regex backend for Google Benchmark on MSYS2 clang64" FORCE)
endif()

FetchContent_MakeAvailable(google_benchmark)
# Google Benchmark FetchContent - End

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

set(SOURCES_NO_MAIN ${SOURCES})
list(FILTER SOURCES_NO_MAIN EXCLUDE REGEX ".*/main\\.cpp$")

add_executable(${PROJECT_NAME} ${SOURCES})

add_executable(bench_lines 
    tests/bench_lines.cpp
    ${SOURCES_NO_MAIN}
)

# ==============================================================================
# COMPILER AND FLAGS
# ==============================================================================

# Flags por target e por configuração (Debug/Release/RelWithDebInfo)
set(PROJ_COMPILE_OPTIONS
    # MSVC - warnings and general adjustments
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX /Zc:__cplusplus /MP 
        /diagnostics:column /FC /Zc:inline>

    # MSVC - configuration-based optimization
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:/Od /Zi>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebInfo>>:/O2 /Zi>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/Ox>

    # GCC/Clang - general warnings
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra 
        -Wno-error=ignored-attributes>

    # GCC/Clang - configuration-based optimization
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Debug>>:-O0 -g3>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:RelWithDebInfo>>:-O2 -g>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3>
)

target_compile_options(${PROJECT_NAME} PRIVATE ${PROJ_COMPILE_OPTIONS})

target_compile_options(bench_lines PRIVATE ${PROJ_COMPILE_OPTIONS})

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

target_link_libraries(${PROJECT_NAME} PRIVATE)

target_include_directories(bench_lines PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_compile_definitions(bench_lines PRIVATE 
    ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/"
)

target_link_libraries(bench_lines PRIVATE
    benchmark::benchmark
    benchmark::benchmark_main
)