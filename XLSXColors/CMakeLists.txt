cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 14)


SET(COMPILE_FLAGS "-O2")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_library(colors OBJECT clsRGBColorRecord.cpp XLSXColorLib.cpp)
