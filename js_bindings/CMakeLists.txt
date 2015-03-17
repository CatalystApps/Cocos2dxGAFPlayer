# this CMakeLists is to generate gafjsbindings.a/gafjsbindings.lib

cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0011 NEW)

project(GAFJSBindings)

set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../../cocos)
if(NOT EXISTS "${COCOS_ROOT}")
  set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../cocos2d-x/cocos)
endif()

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

  ${COCOS_ROOT}
  ${COCOS_ROOT}/base
  ${GAF_PLATFORM_INCLUDE_DIR}
  ${COCOS_ROOT}/../external/glfw3/include/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/zlib/include
  ${COCOS_ROOT}/../../bindings/manual
  ${COCOS_ROOT}/../../external/spidermonkey/include/${PLATFORM_FOLDER}
  ${GAF_ROOT}/Sources
  ${GAF_ROOT}/../js_bindings/bindings
)

file(GLOB_RECURSE gaf_binding_files
  "${CMAKE_SOURCE_DIR}/bindings/*.cpp")
file(GLOB_RECURSE gaf_source_files
  "${GAF_ROOT}/Sources/*.cpp")

add_library(gafjsbindings ${gaf_binding_files} ${gaf_source_files})

#install .a file
INSTALL(
  TARGETS gafjsbindings
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library
)
