cmake_minimum_required(VERSION 3.13)

project(XE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
endif ()

set(LINK_LIBS 
		clog
		bundle
		cpuinfo)
		
if(X_PLATFORM MATCHES "win")
	set(LINK_LIBS ${LINK_LIBS} xinput D3D12MemAlloc)
endif()

if (X_CONFIG MATCHES "debug")
	set(LINK_LIBS ${LINK_LIBS} 
		tbb_debug
		tbbmalloc_debug
		tbbmalloc_proxy_debug)
else ()
	set(LINK_LIBS ${LINK_LIBS} 
		tbb
		tbbmalloc
		tbbmalloc_proxy)
endif ()


FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_library(XE ${X_LIB_TYPE} ${SOURCE_FILES})

target_compile_definitions(XE PRIVATE -DXE_EXPORT)
target_link_directories(XE PRIVATE ${CMAKE_SOURCE_DIR}/depends/lib/${X_PLATFORM}/${X_CONFIG})
target_include_directories(XE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/depends/include)
target_link_libraries(XE PRIVATE ${LINK_LIBS})


install(TARGETS XE
        EXPORT XETargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )

set(PUBLIC_HEADER_DIRS Base Core Inputs Math Memory Network Reflect Utils)
foreach(PUBLIC_HEADER_DIR ${PUBLIC_HEADER_DIRS})
	FILE(GLOB_RECURSE PUBLIC_HEADER_FILES ${PUBLIC_HEADER_DIR}/*.h ${PUBLIC_HEADER_DIR}/*.hpp ${PUBLIC_HEADER_DIR}/*.inc)

	install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include/XE/${PUBLIC_HEADER_DIR}/)
	
	add_custom_command(
		TARGET XE
		PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory ${X_TARGET_DIR}/include/XE/${PUBLIC_HEADER_DIR}/
		COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_HEADER_FILES} ${X_TARGET_DIR}/include/XE/${PUBLIC_HEADER_DIR}/)
endforeach(PUBLIC_HEADER_DIR)

install(FILES XE.h DESTINATION include/XE/)

add_custom_command(
	TARGET XE
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/XE.h ${X_TARGET_DIR}/include/XE/)
	

FILE(GLOB_RECURSE DEPENDS_LIBS ${CMAKE_SOURCE_DIR}/depends/bin/${X_PLATFORM}/${X_CONFIG}/*.*)
add_custom_command(
	TARGET XE
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${DEPENDS_LIBS} ${X_TARGET_DIR}/bin/${X_CONFIG}/)
install(FILES ${DEPENDS_LIBS} DESTINATION bin)