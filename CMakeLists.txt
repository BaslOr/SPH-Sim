cmake_minimum_required(VERSION 3.20)

project(SPH-Sim
    VERSION 1.0
    LANGUAGES CXX
)

# ============================================================
# Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build x64 on Windows
if (WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "SPH-Sim requires a 64-bit build.")
endif()


# ============================================================
# Paths
# ============================================================

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SPH-Sim/src)
set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps)


# ============================================================
# Raylib
# ============================================================

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${DEPS_DIR}/Raylib
    ${CMAKE_BINARY_DIR}/raylib
)


# ============================================================
# Dear ImGui
# ============================================================

set(IMGUI_DIR ${DEPS_DIR}/ImGui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
)


# ============================================================
# rlImGui
# ============================================================

set(RLIMGUI_DIR ${DEPS_DIR}/rlImGui)

add_library(rlimgui STATIC
    ${RLIMGUI_DIR}/rlImGui.cpp
)

target_include_directories(rlimgui PUBLIC
    ${RLIMGUI_DIR}
    ${IMGUI_DIR}
)

target_link_libraries(rlimgui PUBLIC
    raylib
    imgui
)


# ============================================================
# SPH-Sim
# ============================================================

file(GLOB_RECURSE SPH_SOURCES CONFIGURE_DEPENDS
    ${SRC_DIR}/*.cpp
    ${SRC_DIR}/*.h
    ${SRC_DIR}/*.hpp
)

add_executable(SPH-Sim
    ${SPH_SOURCES}
)

target_include_directories(SPH-Sim PRIVATE
    ${SRC_DIR}
)

target_link_libraries(SPH-Sim PRIVATE
    raylib
    imgui
    rlimgui
)


# ============================================================
# Windows
# ============================================================

if (WIN32)
    target_link_libraries(SPH-Sim PRIVATE
        winmm
        gdi32
        opengl32
    )
endif()


# ============================================================
# Visual Studio
# ============================================================

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT SPH-Sim
)