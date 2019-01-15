ADD_EXECUTABLE(traveler 
	traveler.cpp
)

#Need to do more about find library#
FIND_LIBRARY(LIBCLANG_PATH
	clang HINTS /usr/local/lib/
)

TARGET_LINK_LIBRARIES(traveler
	${LIBCLANG_PATH}
)
