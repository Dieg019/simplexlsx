PROJECT(simplexlsx)
subdirs(Xlsx XLSXColors Zip)

cmake_minimum_required(VERSION 3.5.0)
set(CMAKE_CXX_STANDARD 14)


SET(COMPILE_FLAGS "-O2")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

add_library(path_manager OBJECT PathManager.cpp)

add_library(simplexlsx STATIC 
			$<TARGET_OBJECTS:path_manager>
			$<TARGET_OBJECTS:xlsx>
			$<TARGET_OBJECTS:colors>
			$<TARGET_OBJECTS:zip>)
