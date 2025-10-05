# cpp-chess

A C++ chess engine project with a simple raylib-based board renderer.

## Overview

This project starts as a visual sandbox for building a chess engine in C++.
It uses **raylib** for the graphical interface — allowing you to render the board,
highlight moves, and eventually display piece textures and engine evaluations.

The long-term goal is to pair this renderer with a performant bitboard-based
engine and a UCI-compatible backend.

---

## Features

- C++20 / CMake build system  
- Minimal raylib renderer (8×8 board, click-to-select squares)  
- Modular directory layout ready for engine, UI, and assets  
- Cross-platform (macOS, Linux, Windows)  

---

## Build Instructions

### macOS

```bash
brew install cmake raylib
cmake -S . -B build
cmake --build build
./build/chess
```

### Linux (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config libraylib-dev
cmake -S . -B build
cmake --build build
./build/chess
```

### Windows (Visual Studio + vcpkg)

```powershell
# Install raylib
vcpkg install raylib

# Configure and build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\chess.exe
```

---

## Project Layout

```
cpp-chess/
├── src/
│   └── main.cpp        # draws an 8x8 board, click to select
├── include/            # headers (engine code will live here)
├── assets/             # piece sprites later
├── CMakeLists.txt
├── README.md
├── .gitignore
└── LICENSE
```

---

## Dependencies

- **raylib** ≥ 4.5  
- **CMake** ≥ 3.16  
- **C++20 compiler** (clang++, g++, or MSVC)

---

## License

MIT License © 2025 Devon Sawyer

---

## Acknowledgements

- [raylib](https://www.raylib.com) — simple and fun C/C++ game programming library  
- Inspiration from modern open-source engines like [Stockfish](https://stockfishchess.org)  
- Built for experimentation and education
