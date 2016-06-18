cmake_minimum_required(VERSION 3.2)

option(imgstream_build_tests "Build all of imgstream own tests." OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror -pedantic")

project(imgstream)

find_package(png REQUIRED)

set(LIBRARIES "${CMAKE_SOURCE_DIR}/Libraries")
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cc")
file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/test/*.cc")
include_directories("${CMAKE_SOURCE_DIR}/include/")

message("LIBRARIES=" ${LIBRARIES})
message("SOURCES=" ${SOURCES})
message("TEST_SOURCES=" ${TEST_SOURCES})

if(imgstream_build_tests)
### BUILD TESTS ###
include(cmake/TestLib.cmake)
add_executable(imgstreamTest ${SOURCES} ${TEST_SOURCES})
target_link_libraries(imgstreamTest ${PNG_LIBRARIES} libgtest libgmock)
target_include_directories(imgstreamTest PUBLIC ${PNG_INCLUDE_DIRS} libgtest libgmock)
### ########### ###
endif(imgstream_build_tests)

add_library(imgstream STATIC ${SOURCES})
