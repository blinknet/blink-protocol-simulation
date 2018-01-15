cmake_minimum_required(VERSION 3.8)
project(blitz_simulation)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB AUTOJSON_FILES src/third_party/autojson/src/lib/*.cpp src/third_party/autojson/src/lib/*.hpp)
file(GLOB CPP_BASE_FILES src/third_party/cpp-base/src/*.cpp src/third_party/cpp-base/src/*.hpp)

set(THIRD_PARTY_FILES ${AUTOJSON_FILES} ${CPP_BASE_FILES})
set(ALL_SOURCE_FILES ${SOURCE_FILES} ${THIRD_PARTY_FILES})

set(EXTERNAL_THIRD_PARTY_DIR                    "src/third_party")

add_definitions(-Dautojsonuselib)
# thirdparty
if (NOT TARGET autojson)
    add_subdirectory("${EXTERNAL_THIRD_PARTY_DIR}/autojson")
endif ()

add_definitions(-Dcppbaseuselib)
if (NOT TARGET cpp-base)
    add_subdirectory("${EXTERNAL_THIRD_PARTY_DIR}/cpp-base")
endif ()

# all third parties libraries
set(THIRD_PARTY_LIBS "autojson" "cppbase")

set(ALL_SOURCE_FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})
install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}   ${THIRD_PARTY_LIBS} ${CMAKE_THREAD_LIBS_INIT}) # link with third party

add_dependencies(${PROJECT_NAME} autojson)
add_dependencies(${PROJECT_NAME} cppbase)
