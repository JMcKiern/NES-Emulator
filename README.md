# NES Emulator

[![Build Status](https://travis-ci.com/JMcKiern/NES-Emulator.svg?branch=master)](https://travis-ci.com/JMcKiern/NES-Emulator)
[![Build status](https://ci.appveyor.com/api/projects/status/mgv6b0ply361d6nx?svg=true)](https://ci.appveyor.com/project/JMcKiern/nes-emulator)

Another NES emulator written in C++

## How to Build

### Dependencies

Before building this project the following must be installed :
- [GLEW](http://glew.sourceforge.net/) - `libglew-dev` on Ubuntu
- [GLFW](https://www.glfw.org/download.html) - `libglfw3-dev` on Ubuntu

### CMake

To keep this project platform independent CMake is used to generate the build files.

The following definitions can be used to locate the dependencies if they are not found automatically:
- CMAKE_INCLUDE_PATH
- CMAKE_LIBRARY_PATH

### Windows

Use CMake to generate the build files.

It is likely that CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH will need to be defined.

For example

```
cmake -G "Visual Studio 15 2017 Win64" -DCMAKE_INCLUDE_PATH=<path-to-include> -DCMAKE_LIBRARY_PATH=<path-to-libraries> ..
```

### Linux

After the dependencies have been installed run these commands
```
git clone https://github.com/jmckiern/NES-Emulator
cd NES-Emulator
mkdir build
cd build
cmake ..
make
```
