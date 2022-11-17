cmake_minimum_required(VERSION 3.12)
project(CheeseRenderer)
set(CMAKE_CXX_STANDARD 17)
#######################################
## set dependencies

set (BUILD_SHARED_LIBS OFF)

add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/ext)


## test eigen

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/test_eigen/)

add_executable(eigen_test
        ${SRC_FILES}
)
target_include_directories(eigen_test PUBLIC
  ext
  ${STB_INCLUDE}
  ${QOI_INCLUDE}
)
target_link_libraries (eigen_test Eigen3::Eigen)
