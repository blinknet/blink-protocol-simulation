cmake_minimum_required(VERSION 3.8)
project(liquidity_sim)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB AUTOJSON_FILES src/third_party/autojson/src/lib/*.cpp src/third_party/autojson/src/lib/*.hpp)

set(THIRD_PARTY_FILES ${AUTOJSON_FILES})
set(ALL_SOURCE_FILES ${SOURCE_FILES} ${THIRD_PARTY_FILES})

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})
