cmake_minimum_required(VERSION 3.16)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# -------------------------------
# Core library (NO Raylib)
# -------------------------------
add_library(chess_core STATIC
  src/Board.cpp
  src/Pieces.cpp
  src/Attack.cpp
  src/Fen.cpp
)
target_include_directories(chess_core PUBLIC include)

# -------------------------------
# GUI executable (Raylib)
# -------------------------------
find_package(raylib QUIET)
if(NOT raylib_FOUND)
  message(STATUS "raylib not found via find_package. Trying pkg-config...")
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(RAYLIB QUIET raylib)
  endif()
endif()

add_executable(chess
  src/App.cpp
  src/Render.cpp
  src/Input.cpp
  src/Layout.cpp       # defines MakeLayout(...)
  src/Sprites.cpp      # defines SPR_*
  src/main.cpp
)
target_include_directories(chess PRIVATE include)
target_link_libraries(chess PRIVATE chess_core)

if(raylib_FOUND)
  target_link_libraries(chess PRIVATE raylib)
elseif(RAYLIB_FOUND)
  target_include_directories(chess PRIVATE ${RAYLIB_INCLUDE_DIRS})
  target_link_libraries(chess PRIVATE ${RAYLIB_LINK_LIBRARIES})
else()
  message(FATAL_ERROR "raylib not found. Install it (brew install raylib | apt install libraylib-dev | vcpkg).")
endif()

if(APPLE)
  target_link_libraries(chess PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

set_target_properties(chess PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

# -------------------------------
# Perft library + console runner
# -------------------------------
add_library(perft STATIC
  src/Perft.cpp
)
target_include_directories(perft PUBLIC include)
target_link_libraries(perft PUBLIC chess_core)

add_executable(perft_runner
  tests/perft_main.cpp
)
target_include_directories(perft_runner PUBLIC include)
target_link_libraries(perft_runner PRIVATE perft)

set_target_properties(perft_runner PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
