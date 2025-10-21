cmake_minimum_required(VERSION 3.16)
project(BitBoard C)

set(CMAKE_C_STANDARD 11)

add_executable(BitBoard
        main.c
        bitops.c
        game.c
        gui.c
)

# Tell CMake exactly where to find Raylib headers and static lib
include_directories("C:/raylib/raylib/include")
link_directories("C:/raylib/raylib/lib")

# Explicitly link the static library by full path
target_link_libraries(BitBoard
        "C:/raylib/raylib/lib/libraylib.a"
        opengl32
        gdi32
        winmm
)
