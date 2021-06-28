cmake_minimum_required(VERSION 3.16)
project(serio-android)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../../../../.. serio-core)

add_library(${PROJECT_NAME} SHARED api.cpp)
target_link_libraries(${PROJECT_NAME} core)