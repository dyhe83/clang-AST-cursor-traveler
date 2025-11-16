cmake_minimum_required(VERSION 3.14)

add_executable(traveler
    traveler.cpp
)

include_directories(
    ${MAIN_INCLUDE_DIR}
)

# Try to find libclang headers & library (cross-platform)
find_package(Clang QUIET)

find_path(LIBCLANG_INCLUDE
    NAMES clang-c/Index.h
    HINTS
    # Linux / Ubuntu
    /usr/lib/llvm-*/include
    /usr/lib/llvm-18/include
    /usr/include
    /usr/local/include
    /usr/lib/clang/*/include
    # Homebrew macOS
    /opt/homebrew/opt/llvm/include
    /usr/local/opt/llvm/include
    /usr/local/Cellar/llvm/*/include
    # Xcode / CommandLineTools macOS
    /Library/Developer/CommandLineTools/usr/lib/clang/*/include
    /Library/Developer/CommandLineTools/usr/include
    # Windows (common install locations)
    "C:/Program Files/LLVM/include"
    "C:/Program Files (x86)/LLVM/include"
)

find_library(LIBCLANG_LIBRARY
    NAMES clang libclang
    HINTS
    # Linux / Ubuntu
    /usr/lib/llvm-*/lib
    /usr/lib/llvm-18/lib
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib
    # Homebrew macOS
    /opt/homebrew/opt/llvm/lib
    /usr/local/opt/llvm/lib
    /usr/local/Cellar/llvm/*/lib
    # Xcode / CommandLineTools macOS
    /Library/Developer/CommandLineTools/usr/lib
    /Library/Developer/CommandLineTools/usr/include/
    # Windows
    "C:/Program Files/LLVM/bin"
    "C:/Program Files/LLVM/lib"
    "C:/Program Files (x86)/LLVM/lib"
)

if(LIBCLANG_INCLUDE)
    message(STATUS "Found libclang include: ${LIBCLANG_INCLUDE}")
    target_include_directories(traveler PRIVATE ${LIBCLANG_INCLUDE})
elseif(CLANG_FOUND)
    if(CLANG_INCLUDE_DIRS)
        message(STATUS "Using Clang package include dirs: ${CLANG_INCLUDE_DIRS}")
        include_directories(${CLANG_INCLUDE_DIRS})
    endif()
else()
    message(STATUS "libclang headers not found by default hints; you can set LIBCLANG_INCLUDE to the include path.")
endif()

if(LIBCLANG_LIBRARY)
    message(STATUS "Found libclang library: ${LIBCLANG_LIBRARY}")
    target_link_libraries(traveler PRIVATE ${LIBCLANG_LIBRARY} common)
elseif(CLANG_FOUND)
    if(CLANG_LIBRARIES)
        message(STATUS "Using Clang package libraries: ${CLANG_LIBRARIES}")
        target_link_libraries(traveler PRIVATE ${CLANG_LIBRARIES} common)
    else()
        message(FATAL_ERROR "libclang library not found. Install libclang (libclang-dev / LLVM) or set LIBCLANG_LIBRARY.")
    endif()
else()
    message(FATAL_ERROR "libclang library not found. Install libclang (libclang-dev / LLVM) or set LIBCLANG_LIBRARY.")
endif()
