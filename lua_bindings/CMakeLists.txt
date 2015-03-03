# this CMakeLists is to generate gafluabindings.a/gafluabindings.lib

cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0011 NEW)

project(GAFLuaBindings)

set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../../cocos)
set(GAF_ROOT ${CMAKE_SOURCE_DIR}/../Library)

if(WIN32)
  add_definitions(-D_WINDOWS)
  set(PLATFORM_FOLDER win32)
  set(GAF_PLATFORM_INCLUDE_DIR
    ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/gles/include/OGLES
  )
elseif(IOS)
  add_definitions(-DCC_TARGET_OS_IPHONE)
  set(PLATFORM_FOLDER ios)
elseif(MACOSX OR APPLE)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  add_definitions(-DLINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  add_definitions(-DANDROID)
  set(PLATFORM_FOLDER android)
else()
endif()

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -std=c++11")

if(MACOSX OR APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

include_directories(
  /usr/include/
  /usr/local/include/
  /System/Library/Frameworks/
  ${GAF_PLATFORM_INCLUDE_DIR}
  ${GAF_ROOT}/Sources
  ${COCOS_ROOT}/../external/lua/tolua
  ${COCOS_ROOT}/../external/lua/lua
  ${COCOS_ROOT}/scripting/lua-bindings/manual
  ${COCOS_ROOT}
  ${COCOS_ROOT}/2d
  ${COCOS_ROOT}/platform/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/zlib/include
  ${COCOS_ROOT}/../external/glfw3/include/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/base
  ${COCOS_ROOT}/math
)

file(GLOB_RECURSE gaf_binding_files
  "${CMAKE_SOURCE_DIR}/bindings/*.cpp")
file(GLOB_RECURSE gaf_source_files
  "${GAF_ROOT}/Sources/*.cpp")

add_library(gafluabindings ${gaf_binding_files} ${gaf_source_files})
#target_link_libraries(gafluabindings LINK_INTERFACE_LIBRARIES gafplayer)
#install .a file
INSTALL(
  TARGETS gafluabindings
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library
)

#target_link_libraries(gafluabindings gafplayer)
#set_target_properties(gafluabindings
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
#)
