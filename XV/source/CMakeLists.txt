cmake_minimum_required(VERSION 3.13)

project(XV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /MP")
endif ()

set(LINK_LIBS XE)
		
FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_library(XV ${X_LIB_TYPE} ${SOURCE_FILES})

target_compile_definitions(XV PRIVATE -DXV_EXPORT)
target_link_directories(XV PRIVATE ${CMAKE_SOURCE_DIR}/depends/lib/${X_PLATFORM}/${X_CONFIG} ${X_TARGET_DIR}/lib/${X_CONFIG})
target_include_directories(XV PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/depends/include ${X_TARGET_DIR}/include)
target_link_libraries(XV PRIVATE ${LINK_LIBS})


install(TARGETS XV
        EXPORT XVTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )

		
FILE(GLOB_RECURSE PUBLIC_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/*.inc)
install(FILES ${PUBLIC_HEADER_FILES} DESTINATION ${X_TARGET_DIR}/include/XV/)
file(COPY ${PUBLIC_HEADER_FILES} DESTINATION ${X_TARGET_DIR}/include/XV/)

FILE(GLOB_RECURSE CONFIG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../config/*.*)
install(FILES ${CONFIG_FILES} DESTINATION bin)
file(COPY ${CONFIG_FILES} DESTINATION ${X_TARGET_DIR}/bin/${X_CONFIG}/)