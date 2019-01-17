cmake_minimum_required(VERSION 3.9)


project(producerConsumer)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


find_package(Threads)
include_directories("{PROJECT_SOURCE_DIR}")

add_executable(producerConsumer ${PROJECT_SOURCE_DIR}/producerconsumer.cpp)

target_link_libraries (producerConsumer ${CMAKE_THREAD_LIBS_INIT})
