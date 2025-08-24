# Generic System Configuration
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

# Compilation Target Type
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Disable CMake's automatic library and runtime detection
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Disable default libraries and flags
set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARD_LIBRARIES "")

# Compiler and Toolchain Configuration
if(APPLE)
    set(CMAKE_C_COMPILER /opt/local/bin/clang)
    set(CMAKE_CXX_COMPILER /opt/local/bin/clang++)
    set(CMAKE_OBJCOPY /opt/local/bin/llvm-objcopy)
    set(CMAKE_ASM_COMPILER /usr/bin/nasm)
    set (CMAKE_LINKER /opt/local/bin/ld.lld)
elif(UNIX AND NOT APPLE)
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    set(CMAKE_OBJCOPY /usr/bin/llvm-objcopy)
    set(CMAKE_ASM_COMPILER /usr/bin/nasm)
    set(CMAKE_LINKER /usr/bin/ld.lld)
elif(WIN32)
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_OBJCOPY "C:/Program Files/LLVM/bin/llvm-objcopy.exe")
    set(CMAKE_ASM_COMPILER "C:/Program Files/NASM/nasm.exe")
    # Force ELF linker on Windows
    set(CMAKE_LINKER "C:/Program Files/LLVM/bin/ld.lld.exe")
endif()

# Force Compiler Settings
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_ASM_COMPILER_FORCED TRUE)

# Disable Windows-specific behavior
if(WIN32)
    set(CMAKE_C_COMPILE_FEATURES "")
    set(CMAKE_C_STANDARD_LIBRARIES "")
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    # Add flags to avoid Windows dependencies
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostdlib -ffreestanding -fno-builtin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -ffreestanding -fno-builtin")
endif()