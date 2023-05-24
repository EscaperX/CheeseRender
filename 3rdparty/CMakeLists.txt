cmake_minimum_required (VERSION 3.13)


add_subdirectory(pugixml)
set(PUGI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pugixml PARENT_SCOPE)

# set(BUILD_TESTING OFF)
# set(EIGEN_BUILD_DOC OFF)
# add_subdirectory(eigen)
# set(EIGEN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/eigen PARENT_SCOPE)

message(STATUS "GLAD SRC FILES ${GLAD_INCLUDE_DIR}")
# set stb
set(STB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/stb PARENT_SCOPE)

# set imgui
set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui PARENT_SCOPE)

# set obj_loader
add_subdirectory(tinyobjloader)
set(TINYOBJLOADER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyobjloader PARENT_SCOPE)
set(TINYOBJLOADER_LIBRARY_DIR ${CMAKE_INSTALL_LIBDIR}/tinyobjloader PARENT_SCOPE)


add_library(tiny_exr_miniz_lib ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr/miniz/miniz.c)
add_library(tiny_exr_lib ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr/tinyexr.cc)
target_link_libraries(tiny_exr_lib tiny_exr_miniz_lib)
set(TINYEXR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr PARENT_SCOPE)

set(NLOHMANN_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann PARENT_SCOPE)

