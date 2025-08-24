cmake_minimum_required(VERSION 3.20)

project(starkrnl LANGUAGES C CXX ASM_NASM)

set(SUPPORTED_ARCHS "x86" "x86_64")

if(NOT DEFINED TARGET_ARCH)
    message(FATAL_ERROR "Please specify the target architecture using -DTARGET_ARCH=<arch>. Supported: ${SUPPORTED_ARCHS}")
endif()

list(FIND SUPPORTED_ARCHS ${TARGET_ARCH} ARCH_INDEX)
if(ARCH_INDEX EQUAL -1)
    message(FATAL_ERROR "Unsupported architecture: ${TARGET_ARCH}")
endif()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

if(TARGET_ARCH STREQUAL "x86")
    set(CMAKE_SYSTEM_PROCESSOR i386)
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
    set(TARGET_TRIPLE "i386-pc-none-elf")
    set(LLD_EMULATION "elf_i386")
    set(ARCH_NASM_FLAGS "-f elf32")
    set(ARCH_C_FLAGS "-m32")
elseif(TARGET_ARCH STREQUAL "x86_64")
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
    set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)
    set(TARGET_TRIPLE "x86_64-pc-none-elf")
    set(LLD_EMULATION "elf_x86_64")
    set(ARCH_NASM_FLAGS "-f elf64")
    set(ARCH_C_FLAGS "-m64")
else()
    message(FATAL_ERROR "Unsupported TARGET_ARCH: ${TARGET_ARCH}")
endif()

find_program(CLANG_EXECUTABLE clang REQUIRED)
find_program(CLANGXX_EXECUTABLE clang++ REQUIRED)

if(NOT CLANG_EXECUTABLE OR NOT CLANGXX_EXECUTABLE)
    message(FATAL_ERROR "Clang/Clang++ not found. Please install LLVM/Clang.")
endif()

set(CMAKE_C_COMPILER ${CLANG_EXECUTABLE})
set(CMAKE_CXX_COMPILER ${CLANGXX_EXECUTABLE})

set(CMAKE_C_COMPILER_TARGET ${TARGET_TRIPLE})
set(CMAKE_CXX_COMPILER_TARGET ${TARGET_TRIPLE})

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS ON)

set(BASE_FLAGS 
    "--target=${TARGET_TRIPLE}"
    "-ffreestanding"
    "-nostdlib"
    "-nostdinc"
    "-nodefaultlibs"
    "-fno-builtin"
    "-fno-stack-protector"
    "-fno-pic"
    "-fno-pie"
    "-fno-common"
    "-fno-exceptions"
    "-fno-rtti"
    "-fno-unwind-tables"
    "-fno-asynchronous-unwind-tables"
    ${ARCH_C_FLAGS}
)

set(CMAKE_C_FLAGS "${BASE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS "${BASE_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "")
set(CMAKE_CXX_IMPLICIT_LINK_LIBRARIES "")
set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "")
set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_ASM_NASM_FLAGS "${ARCH_NASM_FLAGS}")

set(LINKER_SCRIPT_PATHS
    "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/linker.ld"
    "${CMAKE_SOURCE_DIR}/linker.ld"
)

set(LINKER_SCRIPT "")
foreach(script_path ${LINKER_SCRIPT_PATHS})
    if(EXISTS ${script_path})
        set(LINKER_SCRIPT ${script_path})
        break()
    endif()
endforeach()

if(NOT LINKER_SCRIPT)
    message(FATAL_ERROR "Linker script not found. Searched in: ${LINKER_SCRIPT_PATHS}")
endif()

message(STATUS "Using linker script: ${LINKER_SCRIPT}")

file(GLOB_RECURSE KERNEL_C_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/idt.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/drivers/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/fs/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/fs/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/mm/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/mm/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/proc/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/proc/*.cpp"
)

set(KERNEL_ASM_SRC
    arch/${TARGET_ARCH}/boot.asm
)

add_library(kernel_c_objects OBJECT ${KERNEL_C_SRC})

target_include_directories(kernel_c_objects PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libc
)

target_compile_options(kernel_c_objects PRIVATE
    --target=${TARGET_TRIPLE}
    ${ARCH_C_FLAGS}
    -ffreestanding
    -nostdlib
    -nostdinc
    -nodefaultlibs
    -fno-builtin
    -fno-stack-protector
    -fno-pic
    -fno-pie
    -fno-common
    -fno-exceptions
    -fno-rtti
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -mno-red-zone
    -O2
    -Wall
    -Wextra
    -D__KERNEL__
    -DNDEBUG
)

set_target_properties(kernel_c_objects PROPERTIES
    C_STANDARD 17
    C_EXTENSIONS ON
    POSITION_INDEPENDENT_CODE OFF
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/boot.asm.obj
    COMMAND nasm ${ARCH_NASM_FLAGS} 
        ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/boot.asm 
        -o ${CMAKE_CURRENT_BINARY_DIR}/boot.asm.obj
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/arch/${TARGET_ARCH}/boot.asm
    COMMENT "Assembling boot.asm with NASM"
    VERBATIM
)

add_custom_target(kernel_asm_objects
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/boot.asm.obj
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf
    COMMAND ld.lld
        -T ${LINKER_SCRIPT}
        --entry=start
        --no-dynamic-linker
        -m ${LLD_EMULATION}
        -static
        --nostdlib
        --gc-sections
        $<TARGET_OBJECTS:kernel_c_objects>
        ${CMAKE_CURRENT_BINARY_DIR}/boot.asm.obj
        -o ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf
    DEPENDS kernel_c_objects kernel_asm_objects ${LINKER_SCRIPT}
    COMMENT "Linking kernel with LLD"
    COMMAND_EXPAND_LISTS
)

add_custom_target(kernel ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/kernel.elf
)