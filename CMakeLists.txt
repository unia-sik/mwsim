# $Id: CMakeLists.txt 1643 2016-02-18 07:11:10Z klugeflo $

cmake_minimum_required(VERSION 2.8)
project(mwsim)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -Wextra")
# for gcc 4.8:  -fsanitize=address

#message(STATUS "Binary dir: ${CMAKE_BINARY_DIR}")
#message(STATUS "Source dir: ${CMAKE_SOURCE_DIR}")

set(BINDIR "${PREFIX}/bin")
set(LIBDIR "${PREFIX}/lib")

set(CMAKE_INSTALL_PREFIX $ENV{HOME}/local)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)
include_directories ("${PROJECT_SOURCE_DIR}/src")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)


add_subdirectory(src)
add_subdirectory(doc)
