cmake_minimum_required (VERSION 3.13)
set(CXX_STANDARD_REQUIRED 17)

set(CHEESE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/parse_scene.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/geometry.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/log.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/coordinate.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/intersection.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/render.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/bvh.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/aabb.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
)
find_package(Threads REQUIRED)
add_executable(cheese main.cpp ${CHEESE_BASE})
MESSAGE(STATUS "PUGI_INCLUDE_DIR = ${TINYOBJLOADER_LIBRARY_DIR}")
target_include_directories(cheese PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${TINYEXR_INCLUDE_DIR} ${PUGI_INCLUDE_DIR} ${STB_INCLUDE_DIR} ${TINYOBJLOADER_INCLUDE_DIR} )# ${IMGUI_INCLUDE_DIR}  ${GLFW_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})
target_link_libraries(cheese  PUGI_XML tinyobjloader  Threads::Threads tiny_exr_lib)# GLAD_LIB glfw)# Eigen3::Eigen)
target_link_directories(cheese PRIVATE ${TINYOBJLOADER_LIBRARY_DIR})
