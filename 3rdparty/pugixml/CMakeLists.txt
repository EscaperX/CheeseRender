cmake_minimum_required (VERSION 3.13)

set(PUGI_SRC pugixml.hpp pugixml.cpp pugiconfig.hpp)

add_library(PUGI_XML STATIC ${PUGI_SRC})
