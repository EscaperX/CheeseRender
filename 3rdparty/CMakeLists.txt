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
# set glfw
# set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include PARENT_SCOPE)
# set(GLFW_LINK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/lib-vc2022 PARENT_SCOPE)
# set(GLFW_BUILD_EXAMPLES OFF)
# set(GLFW_BUILD_TESTS OFF)
# set(GLFW_BUILD_DOCS OFF)
# add_subdirectory(glfw)
# set(GLFW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include PARENT_SCOPE)

# # set glad
# set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
# set(GLAD_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c)
# add_library(GLAD_LIB  STATIC ${GLAD_SRC_FILES})
# target_include_directories(GLAD_LIB PUBLIC ${GLAD_INCLUDE_DIR})

add_library(tiny_exr_miniz_lib ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr/miniz/miniz.c)
add_library(tiny_exr_lib ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr/tinyexr.cc)
target_link_libraries(tiny_exr_lib tiny_exr_miniz_lib)
set(TINYEXR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tinyexr PARENT_SCOPE)
