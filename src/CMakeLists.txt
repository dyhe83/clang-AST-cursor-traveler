ADD_EXECUTABLE(traveler 
	traveler.cpp
)

include_directories(
        ${MAIN_INCLUDE_DIR}
)

FIND_LIBRARY(LIBCLANG_PATH
	clang HINTS /usr/local/lib/
)

TARGET_LINK_LIBRARIES(traveler
	${LIBCLANG_PATH}
        common
)
