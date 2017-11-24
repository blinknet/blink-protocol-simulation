cmake_minimum_required(VERSION 3.8)
project(liquidity_sim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES simulate_one_transmission.cpp node.cpp input.cpp event.cpp globals.cpp utils.cpp)
add_executable(liquidity_sim ${SOURCE_FILES})