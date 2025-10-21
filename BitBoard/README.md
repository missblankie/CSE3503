# CS 3503 Assignment 2 - Data Representation and Mapping

## Author
Sara Waymen


## Description
BitBoard Checkers

## Game Instructions

Each player will choose either white or black and then take turns moving their checker pieces. 
To win, players must claim all of their opponents checker pieces. 
If that is not possible the game will be a draw.
White will go first.

*Two player game*

## Controls

- Left mouse click to move pieces
- Use exit button in top right corner to leave the game


## Build Instructions

1. Ensure Visual Studio C++ Build Tools are installed
2. Download Raylib (Installation instructions below)
3. Build
```
rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build .

```
4. Run:

```
./BitBoard

```

## Raylib Installation 

This project uses the [Raylib](https://www.raylib.com/) graphics library.

If Raylib is not already installed, you can download it here:
https://github.com/raysan5/raylib/releases

For Windows + MinGW:
- Download **raylib-5.5_win64_mingw.zip**
- Extract it to 'C:\raylib\raylib'
  (you should have `C:\raylib\raylib\include\raylib.h` and `C:\raylib\raylib\lib\libraylib.a`)

The included `CMakeLists.txt` is already configured to look for this directory.
