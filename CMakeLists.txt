# EVF9 - Extended Vehicle Framework 9
#
# Created by EAST9
# Open.MP / SA-MP Community
#
# GitHub: https://github.com/east9-777
# Discord: leivison0230_58048
# Comunidade Discord: https://discord.gg/FJBWAesQV8
#
# Copyright (c) EAST9
# Do not remove this notice.

cmake_minimum_required(VERSION 3.16)
project(evf9 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# SDK path — clone before building:
#   git submodule update --init --recursive
# Or provide your own path:
#   cmake -DAMP_SDK_DIR=/path/to/sdk ..
#   cmake -DSAMP_SDK_DIR=/path/to/sdk ..
# ---------------------------------------------------------------------------

option(EVF9_BUILD_SAMP "Compile for SA-MP (uses SAMP_SDK_DIR)" OFF)

if(EVF9_BUILD_SAMP)
    message(STATUS "[EVF9] Building for SA-MP")
    if(NOT DEFINED SAMP_SDK_DIR)
        set(SAMP_SDK_DIR "${CMAKE_SOURCE_DIR}/sdk/samp")
    endif()
    if(NOT EXISTS "${SAMP_SDK_DIR}/amx/amx.h" AND NOT EXISTS "${SAMP_SDK_DIR}/include/amx/amx.h")
        # Try the sdk root itself
        if(EXISTS "${SAMP_SDK_DIR}/plugincommon.h")
            # Flat layout
        else()
            message(FATAL_ERROR
                "[EVF9] SA-MP SDK not found at ${SAMP_SDK_DIR}\n"
                "Provide: cmake -DSAMP_SDK_DIR=/path/to/samp-sdk ..\n"
                "The SDK must contain amx/amx.h and plugincommon.h"
            )
        endif()
    endif()
    set(EVF9_SDK_DIR "${SAMP_SDK_DIR}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -fPIC")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
else()
    message(STATUS "[EVF9] Building for Open.MP (64-bit)")
    if(NOT DEFINED AMP_SDK_DIR)
        set(AMP_SDK_DIR "${CMAKE_SOURCE_DIR}/sdk/omp")
    endif()
    if(NOT EXISTS "${AMP_SDK_DIR}/include/amx/amx.h" AND NOT EXISTS "${AMP_SDK_DIR}/amx/amx.h")
        message(FATAL_ERROR
            "[EVF9] Open.MP SDK not found at ${AMP_SDK_DIR}\n"
            "Run:   git submodule update --init --recursive\n"
            "  Or:  git clone https://github.com/openmultiplayer/open.mp-sdk sdk/omp\n"
            "  Or:  cmake -DAMP_SDK_DIR=/path/to/omp-sdk .."
        )
    endif()
    set(EVF9_SDK_DIR "${AMP_SDK_DIR}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# ---------------------------------------------------------------------------
# Debug options
# ---------------------------------------------------------------------------
option(EVF9_DEBUG "Enable EVF9 debug logging" OFF)
if(EVF9_DEBUG)
    add_compile_definitions(EVF9_ENABLE_DEBUG=1)
endif()

# ---------------------------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers)
elseif(MSVC)
    add_compile_options(/W3 /wd4100 /wd4244)
endif()

# ---------------------------------------------------------------------------
# Sources — all .cpp files under src/ (recursive)
# ---------------------------------------------------------------------------
file(GLOB_RECURSE EVF9_SOURCES
    "src/*.cpp"
    "src/natives/*.cpp"
    "src/natives/modules/*.cpp"
)

# ---------------------------------------------------------------------------
# Include paths
# ---------------------------------------------------------------------------
include_directories(
    "${EVF9_SDK_DIR}/include"
    "${EVF9_SDK_DIR}"          # flat SA-MP SDK layout
    "${CMAKE_SOURCE_DIR}/src"
)

# ---------------------------------------------------------------------------
# Shared library target
# ---------------------------------------------------------------------------
add_library(evf9 SHARED ${EVF9_SOURCES})

set_target_properties(evf9 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "evf9"
)

# On Windows the plugin is a .dll, on Linux a .so
if(WIN32)
    set_target_properties(evf9 PROPERTIES SUFFIX ".dll")
else()
    set_target_properties(evf9 PROPERTIES SUFFIX ".so")
endif()

# ---------------------------------------------------------------------------
# Link options
# ---------------------------------------------------------------------------
if(UNIX)
    # Remove unused sections; do not link against system default libs
    target_link_options(evf9 PRIVATE
        -Wl,--no-undefined
        -Wl,--gc-sections
    )
endif()

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
install(TARGETS evf9 DESTINATION plugins)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/pawno/include/EVF9"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/pawno/include/EVF9")

message(STATUS "[EVF9] Configured. Sources: ${EVF9_SOURCES}")
