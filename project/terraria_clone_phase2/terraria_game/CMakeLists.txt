cmake_minimum_required(VERSION 3.15)
project(terraria_clone C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Fetch raylib automatically.
#
# This means you do NOT need to install raylib yourself first: CMake will
# download the raylib source and build it as part of this project the
# first time you configure. This is the simplest path for Visual Studio's
# "Open Folder" (CMake) workflow on Windows.
# ---------------------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
)

# Options for raylib's own CMake build - we only need the library, not
# raylib's examples/games.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

# ---------------------------------------------------------------------------
# Our game
# ---------------------------------------------------------------------------
 
# Pull in every .c file under src/ automatically, so new modules added in
# later phases (world.c, player.c, ...) don't require editing this file.
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# On Windows, raylib/GLFW need a couple of extra system libraries.
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm gdi32 opengl32)
endif()

# On Linux, we need X11, math, dl, pthread, rt.
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE m pthread dl rt X11)
endif()

if (APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

# So DrawText() etc. can find assets using relative paths ("assets/...")
# no matter which IDE/build folder runs the executable from.
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
