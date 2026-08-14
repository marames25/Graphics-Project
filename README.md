# Graphics-Project

A C++ graphics project for drawing and manipulating geometric shapes using the Windows API.

## Description
This project implements several graphics algorithms for lines, circles, ellipses, clipping, and polygon/face-related drawing operations.

## Features
- DDA line drawing
- midpoint line drawing
- parametric line drawing
- direct, polar, iterative, and midpoint circle algorithms
- ellipse drawing algorithms
- clipping operations
- drawing geometric primitives
- Windows-based graphics interface

## Technologies Used
- C++
- Windows API
- GDI
- CMake

## Prerequisites
- CMake
- C++ compiler
- Windows environment

## Installation
```bash
cmake -S . -B build
cmake --build build
```

## How to Run
```bash
./build/main
```

On Windows:
```powershell
.\build\main.exe
```

## Project Structure
- `main.cpp`
- `Algorithms.cpp`
- `Algorithms.h`
- `globals.h`
- `CMakeLists.txt`

## Notes
This project is Windows-specific and uses Win32 GDI drawing calls.
