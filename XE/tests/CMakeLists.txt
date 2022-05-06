cmake_minimum_required(VERSION 3.13)

project(XETests)
set(CMAKE_CXX_STANDARD 17)


FILE(GLOB_RECURSE SOURCE_FILES *.*)
add_executable(XETests ${SOURCE_FILES})

target_include_directories(XETests PRIVATE  ${CMAKE_SOURCE_DIR}/target/include/XE ${CMAKE_SOURCE_DIR}/depends/include)
target_link_directories(XETests PRIVATE ${CMAKE_SOURCE_DIR}/depends/lib/${X_PLATFORM}/${X_CONFIG})

target_link_libraries(XETests PRIVATE XE gtest)