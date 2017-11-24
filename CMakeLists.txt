cmake_minimum_required(VERSION 3.8)
project(liquidity_sim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/simulate_one_transmission.cpp src/node.cpp src/input.cpp src/globals.cpp src/utils.cpp src/heap.cpp)
add_executable(liquidity_sim ${SOURCE_FILES})
