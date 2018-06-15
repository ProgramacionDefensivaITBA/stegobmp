cmake_minimum_required(VERSION 3.10)
project(stegobmp C)

set(CMAKE_C_STANDARD 99)

add_executable(stegobmp main.c bmp.c bmp.h stegobmp.c stegobmp.h)