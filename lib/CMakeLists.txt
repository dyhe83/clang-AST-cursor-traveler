ADD_LIBRARY(common STATIC
        Node.cpp
        )

include_directories(
        ${MAIN_INCLUDE_DIR}
)

# If parent CMake located libclang include, add it for the common library
if(DEFINED LIBCLANG_INCLUDE)
        target_include_directories(common PRIVATE ${LIBCLANG_INCLUDE})
elseif(DEFINED CLANG_INCLUDE_DIRS)
        target_include_directories(common PRIVATE ${CLANG_INCLUDE_DIRS})
endif()
