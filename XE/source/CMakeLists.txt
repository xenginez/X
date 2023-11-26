cmake_minimum_required(VERSION 3.13)

project(XE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif ()

FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_library(XE ${X_LIB_TYPE} ${SOURCE_FILES})

target_compile_definitions(XE PRIVATE -DXE_EXPORT)
target_link_directories(XE PRIVATE ${CMAKE_SOURCE_DIR}/depends/lib/${X_PLATFORM}/${X_CONFIG})
target_include_directories(XE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/depends/include)
target_link_libraries(XE PRIVATE clog cpuinfo OpenAL32)


install(TARGETS XE
        EXPORT XETargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )

set(PUBLIC_HEADER_DIRS Audio Base Core GUI Input Math Memory Network Physics Reflect Render Scene Utils WASM)
foreach(PUBLIC_HEADER_DIR ${PUBLIC_HEADER_DIRS})
	FILE(GLOB_RECURSE PUBLIC_HEADER_FILES ${PUBLIC_HEADER_DIR}/*.h ${PUBLIC_HEADER_DIR}/*.hpp ${PUBLIC_HEADER_DIR}/*.inc)
	install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include/XE/${PUBLIC_HEADER_DIR}/)
endforeach(PUBLIC_HEADER_DIR)

install(FILES XE.h DESTINATION include/XE/)

FILE(GLOB_RECURSE DEPEND_LIBS ${CMAKE_SOURCE_DIR}/depends/bin/${X_PLATFORM}/${X_CONFIG}/*.*)
install(FILES ${DEPEND_LIBS} DESTINATION bin)