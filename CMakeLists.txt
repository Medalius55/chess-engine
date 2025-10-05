cmake_minimum_required(VERSION 3.16)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(raylib QUIET)
if(NOT raylib_FOUND)
  message(STATUS "raylib not found via find_package. Trying pkg-config...")
  find_package(PkgConfig QUIET)
  if(PkgConfig_FOUND)
    pkg_check_modules(RAYLIB QUIET raylib)
  endif()
endif()

file(GLOB SRC
  src/*.cpp
)

add_executable(chess ${SRC})

if(raylib_FOUND)
  target_link_libraries(chess PRIVATE raylib)
elseif(RAYLIB_FOUND)
  target_include_directories(chess PRIVATE ${RAYLIB_INCLUDE_DIRS})
  target_link_libraries(chess PRIVATE ${RAYLIB_LINK_LIBRARIES})
else()
  message(FATAL_ERROR "raylib not found. Install it (brew install raylib | apt install libraylib-dev | vcpkg).")
endif()

target_include_directories(chess PRIVATE include)

if(APPLE)
  target_link_libraries(chess PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
endif()

set_target_properties(chess PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
