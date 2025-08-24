cmake_minimum_required(VERSION 3.10)

project(stosinit LANGUAGES C)

set(CMAKE_C_STANDARD 11)

include_directories(../libc)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-stack-protector -fno-pic -fno-pie --target=i386-pc-none-elf")

add_library(stosinit_obj OBJECT main.c)

add_custom_target(stosinit ALL
    COMMAND "C:/Program Files/LLVM/bin/ld.lld.exe"
        --entry=_start
        -static
        -nostdlib
        -o ${CMAKE_CURRENT_BINARY_DIR}/stosinit.exe
        $<TARGET_OBJECTS:stosinit_obj>
        ${CMAKE_CURRENT_BINARY_DIR}/../libc/liblibc.a
    DEPENDS stosinit_obj libc
    COMMENT "Linking stosinit with ld.lld"
    VERBATIM
)