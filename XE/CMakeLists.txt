cmake_minimum_required(VERSION 3.13)

add_subdirectory(source)
add_subdirectory(launcher)

if(X_BUILD_TESTS)
	add_subdirectory(tests)
endif()