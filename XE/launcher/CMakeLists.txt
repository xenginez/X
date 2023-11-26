cmake_minimum_required(VERSION 3.13)

project(XELauncher)
set(CMAKE_CXX_STANDARD 20)
add_executable(XELauncher "windows_main.cpp" "linux_main.cpp" "mac_main.mm" "ios_main.mm"  "android_main.cpp")

target_include_directories(XELauncher PRIVATE ${CMAKE_SOURCE_DIR}/XE/source)
target_link_libraries(XELauncher PRIVATE XE)

install(TARGETS XELauncher
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )
