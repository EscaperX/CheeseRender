cmake_minimum_required (VERSION 3.13)
set(CXX_STANDARD_REQUIRED 17)

set(CHEESE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/scene_parser/parse_scene.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/scene_parser/parse_scene_json.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/scene_parser/parse_scene_xml.cpp
                ${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
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
                PARENT_SCOPE
)
