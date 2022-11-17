cmake_minimum_required(VERSION 3.12)

set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/drawImage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/generalInterfaceExamples.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/generalInterfaceTiledExamples.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/lowLevelIoExamples.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/previewImageExamples.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rgbaInterfaceExamples.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rgbaInterfaceTiledExamples.cpp
        PARENT_SCOPE
)
set(SRC_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        PARENT_SCOPE
)