# this CMakeLists is to generate gafluabindings.a/gafluabindings.lib

cmake_minimum_required(VERSION 2.6)

project(GAFLuaBindings)

set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../../cocos)
set(GAF_ROOT ${CMAKE_SOURCE_DIR}/../Library)

include(${GAF_ROOT}/CMakeLists.txt)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -std=c++11")

if(MACOSX OR APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

include_directories(
#  /usr/include/
#  /usr/local/include/
#  /System/Library/Frameworks/
  ${GAF_ROOT}/Sources
  ${COCOS_ROOT}/../external/lua/tolua
  ${COCOS_ROOT}/../external/lua/lua
  ${COCOS_ROOT}/scripting/lua-bindings/manual
)

file(GLOB_RECURSE gaf_binding_files
  "${CMAKE_SOURCE_DIR}/bindings/*.cpp")

add_library(gafluabindings ${gaf_binding_files})

#install .a file
INSTALL(
  TARGETS gafluabindings
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library
)

