cmake_minimum_required(VERSION 3.20)

project(StarOS)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib --target=i386-pc-none-elf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib --target=i386-pc-none-elf")

add_subdirectory(libc)

add_subdirectory(kernel)

add_subdirectory(stosinit)