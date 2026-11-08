# Picket — root build file.
# One file describes every target. Targets are added here step by step as their code appears.

cmake_minimum_required(VERSION 3.28)   # 3.28 gives FetchContent's EXCLUDE_FROM_ALL

# C is listed because raylib is written in C; our own code is C++.
project(picket VERSION 0.1.0 LANGUAGES C CXX)

# --- Language rules for every C++ target in this project ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)   # fail loudly instead of quietly using an older standard
set(CMAKE_CXX_EXTENSIONS OFF)         # -std=c++20, not gnu++20: no GCC-only extensions sneak in

# Warnings for OUR targets only. Third-party code keeps its own settings.
function(picket_warnings target)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wshadow)
endfunction()

# --- Third-party libraries, downloaded once at configure time ---
include(FetchContent)

# raylib: window, 2D drawing, input. Only the game target will link it.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)       # skip raylib's 100+ example programs
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)   # WSLg shows X11 windows; Wayland needs extra packages
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        6.0
    GIT_SHALLOW    TRUE
    EXCLUDE_FROM_ALL
)

# GoogleTest: the test framework. Only the test target will link it.
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)     # we need gtest, not the mocking library
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)   # never install it system-wide
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
    GIT_SHALLOW    TRUE
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(raylib googletest)

enable_testing()

# --- Our targets (added in later steps) ---
# sim/      -> picket_sim       static library, NEVER links raylib
# headless/ -> picket_headless  links picket_sim
# game/     -> picket           links picket_sim + raylib
# tests/    -> picket_tests     links picket_sim + GoogleTest