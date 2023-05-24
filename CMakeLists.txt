cmake_minimum_required (VERSION 3.13)
project(Cheese)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
# add_compile_options("/wd 4819")


add_subdirectory(3rdparty)
# find_package(Eigen3 REQUIRED)



add_subdirectory(src)

find_package(Threads REQUIRED)

include_directories( ${NLOHMANN_INCLUDE_DIR} ${TINYEXR_INCLUDE_DIR} ${PUGI_INCLUDE_DIR} ${STB_INCLUDE_DIR} ${TINYOBJLOADER_INCLUDE_DIR} )

add_executable(cheese main.cpp ${CHEESE_BASE})
MESSAGE(STATUS "PUGI_INCLUDE_DIR = ${TINYOBJLOADER_LIBRARY_DIR}")
target_include_directories(cheese PRIVATE include/ ${CMAKE_CURRENT_SOURCE_DIR})# ${3RDPARTY_INCLUDE})
target_link_libraries(cheese  PUGI_XML tinyobjloader  Threads::Threads tiny_exr_lib)# GLAD_LIB glfw)# Eigen3::Eigen)
target_link_directories(cheese PRIVATE ${TINYOBJLOADER_LIBRARY_DIR})

