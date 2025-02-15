cmake_minimum_required(VERSION 3.10)
project(Allocator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

