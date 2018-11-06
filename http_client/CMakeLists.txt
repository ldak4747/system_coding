cmake_minimum_required(VERSION 2.8)

project(data-access)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILECMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O2 -Wall -g -ggdb")
set(_GLIBCXX_USE_NANOSLEEP ON)
set(CMAKE_BUILD_TYPE "Debug")

set(ROOT_DIR "/usr/local")
set(NLOHMANN_JSON "${ROOT_DIR}/include/nlohmann")

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BASIC_DIR "/data1/liudong8/study")
set(GCC_BASE_DIR "${BASIC_DIR}/gcc7.3")
set(CMAKE_C_COMPILER "${GCC_BASE_DIR}/bin/gcc")
set(CMAKE_CXX_COMPILER "${GCC_BASE_DIR}/bin/g++")

include_directories(${NLOHMANN_JSON})

set(HTTPCLIENT_SRC HttpClient.cc main.cc)
add_executable(httpclient ${HTTPCLIENT_SRC})

