cmake_minimum_required(VERSION 3.19)
project(evf9 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# EVF9 targets Open.MP ONLY — 64-bit Linux x64 and Windows x64.
# SA-MP support has been completely removed.
#
# Dependencies (all under sdk/):
#   sdk/omp/   — open.mp SDK (git submodule: openmultiplayer/open.mp-sdk)
#                Submodules inside sdk/omp/lib/ must also be present:
#                  lib/glm, lib/robin-hood-hashing, lib/span-lite, lib/string-view-lite
#   sdk/amx/   — AMX machine headers (amx.h) from pawn-lang/compiler
#
# Build:
#   mkdir build && cd build
#   cmake ..
#   cmake --build . -j
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Open.MP SDK path
# ---------------------------------------------------------------------------
if(NOT DEFINED AMP_SDK_DIR)
    set(AMP_SDK_DIR "${CMAKE_SOURCE_DIR}/sdk/omp")
endif()

if(NOT EXISTS "${AMP_SDK_DIR}/include/sdk.hpp")
    message(FATAL_ERROR
        "[EVF9] Open.MP SDK not found at ${AMP_SDK_DIR}/include/sdk.hpp\n"
        "Run:  git submodule update --init --recursive\n"
        "  Or: cmake -DAMP_SDK_DIR=/path/to/open.mp-sdk .."
    )
endif()

message(STATUS "[EVF9] Open.MP SDK found at: ${AMP_SDK_DIR}")

# ---------------------------------------------------------------------------
# AMX headers path — sdk/amx/amx.h (from pawn-lang/compiler)
# Included as <amx/amx.h> by the SDK's pawn.hpp.
# ---------------------------------------------------------------------------
set(AMX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/sdk")
if(NOT EXISTS "${AMX_INCLUDE_DIR}/amx/amx.h")
    message(FATAL_ERROR
        "[EVF9] AMX headers not found at ${AMX_INCLUDE_DIR}/amx/amx.h\n"
        "Copy amx.h from pawn-lang/compiler source/amx/ into sdk/amx/"
    )
endif()
message(STATUS "[EVF9] AMX headers found at: ${AMX_INCLUDE_DIR}/amx/")

# ---------------------------------------------------------------------------
# Add the SDK as a CMake subdirectory.
# This defines the OMP-SDK INTERFACE target which pulls in:
#   glm::glm, robin_hood, span-lite, string-view-lite
# and their required compile definitions (GLM_FORCE_QUAT_DATA_WXYZ, etc.)
# ---------------------------------------------------------------------------
add_subdirectory("${AMP_SDK_DIR}" omp-sdk EXCLUDE_FROM_ALL)

# ---------------------------------------------------------------------------
# Optional debug logging
# ---------------------------------------------------------------------------
option(EVF9_DEBUG "Enable EVF9 debug logging" OFF)
if(EVF9_DEBUG)
    add_compile_definitions(EVF9_ENABLE_DEBUG=1)
endif()

# ---------------------------------------------------------------------------
# Compiler flags — 64-bit only, no -m32
# ---------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -fvisibility=hidden
        -fPIC
    )
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
# Shared library target
# ---------------------------------------------------------------------------
add_library(evf9 SHARED ${EVF9_SOURCES})

target_include_directories(evf9 PRIVATE
    "${AMP_SDK_DIR}/include"          # sdk.hpp, component.hpp, core.hpp, pawn.hpp …
    "${AMX_INCLUDE_DIR}"              # amx/amx.h  (included as <amx/amx.h>)
    "${CMAKE_SOURCE_DIR}/src"         # internal plugin headers
)

# HAVE_STDINT_H: tells amx.h to use <stdint.h> for fixed-width types.
# This avoids the legacy platform-detection fallbacks in the pawn-lang header.
target_compile_definitions(evf9 PRIVATE
    HAVE_STDINT_H=1
)

# Link the OMP-SDK INTERFACE target (provides glm, robin_hood, span-lite, etc.)
target_link_libraries(evf9 PRIVATE OMP-SDK)

set_target_properties(evf9 PROPERTIES
    PREFIX ""
    OUTPUT_NAME "evf9"
    CXX_VISIBILITY_PRESET   hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if(WIN32)
    set_target_properties(evf9 PROPERTIES SUFFIX ".dll")
else()
    set_target_properties(evf9 PROPERTIES SUFFIX ".so")
    target_link_options(evf9 PRIVATE
        # --no-undefined is intentionally omitted: amx_GetAddr, amx_GetString,
        # amx_SetString etc. are resolved at runtime from the open.mp server's
        # symbol table when the component .so is loaded — they are NOT part of
        # any static library we link against.
        -Wl,--gc-sections
    )
endif()

# ---------------------------------------------------------------------------
# Install — OMP loads components from "components/", not "plugins/"
# ---------------------------------------------------------------------------
install(TARGETS evf9 DESTINATION components)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/pawno/include/EVF9"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/pawno/include/EVF9"
        OPTIONAL)

message(STATUS "[EVF9] Configured for Open.MP x64. Sources: ${EVF9_SOURCES}")
