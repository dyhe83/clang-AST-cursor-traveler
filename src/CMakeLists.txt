ADD_EXECUTABLE(traveler 
	traveler.cpp
)

include_directories(
        ${MAIN_INCLUDE_DIR}
)

FIND_LIBRARY(LIBCLANG_PATH
	clang HINTS /usr/local/lib/ /usr/lib/llvm-18/lib /usr/lib/x86_64-linux-gnu/
)

if(NOT LIBCLANG_PATH)
	message(FATAL_ERROR "libclang not found. Please install libclang-dev.")
endif()

TARGET_LINK_LIBRARIES(traveler
	${LIBCLANG_PATH}
        common
)
