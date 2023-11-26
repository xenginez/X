cmake_minimum_required(VERSION 3.5)

project(XSLauncher VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_executable(XSLauncher ${SOURCE_FILES})

target_include_directories(XSLauncher PRIVATE ${X_TARGET_DIR}/include)
target_link_directories(XSLauncher PRIVATE ${X_TARGET_DIR}/lib/${X_CONFIG})
target_link_libraries(XSLauncher PRIVATE XE XS Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS XSLauncher
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )
