cmake_minimum_required (VERSION 3.13)
project(Cheese)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
# add_compile_options("/wd 4819")


add_subdirectory(3rdparty)
# find_package(Eigen3 REQUIRED)



add_subdirectory(src)
