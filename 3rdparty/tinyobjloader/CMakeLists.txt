cmake_minimum_required (VERSION 3.13)

set(TINYOBJLOADER_SRC tiny_obj_loader.cc tiny_obj_loader.h)

add_library(tinyobjloader STATIC ${TINYOBJLOADER_SRC})
