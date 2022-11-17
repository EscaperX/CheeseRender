cmake_minimum_required (VERSION 3.12)

# For sanitizers

## eigen
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/eigen)

# stb

set (STB_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/stb PARENT_SCOPE)

########################################################
# zlib

find_package (ZLIB)
if (NOT ZLIB_FOUND)
  # Build zlib
  set (ZLIB_BUILD_STATIC_LIBS ON CACHE BOOL " " FORCE)
  set (ZLIB_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
  add_subdirectory (zlib)

  set (ZLIB_LIBRARIES zlibstatic)
  set (ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib)

  # try to make openexr happy about this...
  set (ZLIB_LIBRARY zlibstatic)
  set (ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib)
  set (ZLIB_FOUND TRUE)

  set_property (TARGET zlibstatic PROPERTY FOLDER "ext")

  add_library (ZLIB::ZLIB ALIAS zlibstatic)
  include_directories(${ZLIB_INCLUDE_DIRS})  # yuck, but so openexr/ptex can find zlib.h...
endif ()

set (ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS} PARENT_SCOPE)
set (ZLIB_LIBRARIES ${ZLIB_LIBARIES} PARENT_SCOPE)

###########################################################################
# OpenEXR

find_package (OpenEXR 2.3) # 2.3 is the first version compatible with C++17
message (STATUS "Build OpenEXR")

if (OPENEXR_FOUND)
  message (STATUS "Found OpenEXR version ${OPENEXR_VERSION}")
  set (OPENEXR_INCLUDE ${OPENEXR_INCLUDES}/OpenEXR ${ILMBASE_INCLUDES}/OpenEXR PARENT_SCOPE)
  if (WIN32)
    set (OPENEXR_LIBS ${OPENEXR_LIBRARIES} ${ZLIB_LIBRARY} PARENT_SCOPE)
  else ()
    set (OPENEXR_LIBS ${OPENEXR_LIBRARIES} PARENT_SCOPE)
  endif ()
else ()
  message (STATUS "OpenEXR not found; building it from scratch.")

  SET (CMAKE_POLICY_DEFAULT_CMP0077 NEW)

  set (ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
  set (OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
  set (OPENEXR_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
  set (ILMBASE_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
  set (PYILMBASE_ENABLE             OFF CACHE BOOL " " FORCE)
  set (OPENEXR_BUILD_UTILS          OFF CACHE BOOL " " FORCE)

  add_subdirectory (openexr)

  set_property (TARGET IexMath IlmThread Half
                Iex Imath IlmImf HalfTest IexTest
                IlmImfExamples IlmImfTest IlmImfUtil IlmImfUtilTest ImathTest
                PROPERTY FOLDER "ext/OpenEXR")

  set (OPENEXR_INCLUDE
    ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase/Imath
    ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase/Half
    ${CMAKE_CURRENT_SOURCE_DIR}/openexr/IlmBase/Iex
    ${CMAKE_CURRENT_SOURCE_DIR}/openexr/OpenEXR/IlmImf
    ${CMAKE_CURRENT_BINARY_DIR}/openexr/IlmBase/config
    ${CMAKE_CURRENT_BINARY_DIR}/openexr/OpenEXR/config
  PARENT_SCOPE
  )
  

  if (WIN32)
    set (OPENEXR_LIBS OpenEXR::IlmImf IlmBase::Imath IlmBase::Half ${ZLIB_LIBRARY} PARENT_SCOPE)
  else ()
    set (OPENEXR_LIBS OpenEXR::IlmImf IlmBase::Imath IlmBase::Half PARENT_SCOPE)
  endif ()
endif ()

###########################################################################