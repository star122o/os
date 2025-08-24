cmake_minimum_required(VERSION 3.20)

project(libc LANGUAGES C CXX)

add_library(libc STATIC)
target_sources(libc PRIVATE
    stdio.cpp
    ../kernel/drivers/io.c
)


target_include_directories(libc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../kernel/include 
)

target_compile_options(libc PRIVATE --target=i386-pc-none-elf)