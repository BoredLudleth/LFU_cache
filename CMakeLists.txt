cmake_minimum_required (VERSION 3.0)

project (cache)

set (CMAKE_CXX_STANDARD 20)

set (SOURCES src/main.cpp)
set (HEADERS include/lfu_cache.hpp include/perfect_cache.hpp)

include_directories(include/)

add_executable (main src/main.cpp include/lfu_cache.hpp include/perfect_cache.hpp)
