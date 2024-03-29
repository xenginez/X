cmake_minimum_required(VERSION 3.5)

project(XS)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_library(XS ${X_LIB_TYPE} ${SOURCE_FILES})

target_compile_definitions(XS PRIVATE -DXS_EXPORT)
target_link_directories(XS PRIVATE ${CMAKE_SOURCE_DIR}/depends/lib/${X_PLATFORM}/${X_CONFIG})
target_include_directories(XS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/depends/include ${CMAKE_SOURCE_DIR}/XE/source)
target_link_libraries(XS PRIVATE XE Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS XS
        EXPORT XSTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../resource DESTINATION bin)

if(${X_CONFIG} MATCHES "release")
    set(PUBLIC_HEADER_DIRS Core Widgets)
    foreach(PUBLIC_HEADER_DIR ${PUBLIC_HEADER_DIRS})
	    FILE(GLOB_RECURSE PUBLIC_HEADER_FILES ${PUBLIC_HEADER_DIR}/*.h ${PUBLIC_HEADER_DIR}/*.hpp ${PUBLIC_HEADER_DIR}/*.inc)
        install(FILES ${PUBLIC_HEADER_FILES} DESTINATION include/XS/${PUBLIC_HEADER_DIR})
    endforeach(PUBLIC_HEADER_DIR)
    install(FILES XS.h DESTINATION include/XS/)
endif()