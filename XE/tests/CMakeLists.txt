cmake_minimum_required(VERSION 3.13)

project(XETests)
set(CMAKE_CXX_STANDARD 17)


FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_executable(XETests ${SOURCE_FILES})

target_include_directories(XETests PRIVATE ${X_TARGET_DIR}/include)
target_link_directories(XETests PRIVATE ${X_TARGET_DIR}/lib/${X_CONFIG})

if (X_CONFIG MATCHES "debug")
target_link_libraries(XETests PRIVATE XE gtestd)
else ()
target_link_libraries(XETests PRIVATE XE gtest)
endif ()
