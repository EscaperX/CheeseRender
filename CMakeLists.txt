cmake_minimum_required(VERSION 3.12)
project(CheeseRenderer)
set(CMAKE_CXX_STANDARD 17)
#######################################
## set dependencies

set (BUILD_SHARED_LIBS OFF)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/ext)


## test eigen

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/test_eigen/)

add_executable(eigen_test ${SRC_FILES})
target_include_directories(eigen_test PUBLIC ext)
target_link_libraries (eigen_test Eigen3::Eigen)


## test SDL2

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/test_sdl)
add_executable(sdl_test ${TEST_SDL_SRC_FILES})
target_include_directories(sdl_test PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(sdl_test ${SDL2_LIBRARY})
