cmake_minimum_required(VERSION 3.10)
project(stegobmp C)

set(CMAKE_C_STANDARD 99)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(stegobmp main.c bmp.c bmp.h stegobmp.c stegobmp.h crypt.c crypt.h)
target_link_libraries(stegobmp ${OPENSSL_LIBRARIES})