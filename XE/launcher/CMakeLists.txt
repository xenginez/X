cmake_minimum_required(VERSION 3.13)

project(XELauncher)
set(CMAKE_CXX_STANDARD 17)
add_executable(XELauncher "windows_main.cpp" "linux_main.cpp" "mac_main.mm" "ios_main.mm"  "android_main.cpp")

target_include_directories(XELauncher PRIVATE ${X_TARGET_DIR}/include)
target_link_directories(XELauncher PRIVATE ${X_TARGET_DIR}/lib/${X_CONFIG})
target_link_libraries(XELauncher PRIVATE XE)