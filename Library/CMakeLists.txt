#/****************************************************************************
# Copyright (c) 2013 cocos2d-x.org
# Copyright (c) 2014 martell malone
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

# this CMakeLists is to generate GAFPlayer.a/GAFPlayer.lib

cmake_minimum_required(VERSION 2.6)

# project(GAFPlayer)

set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../../cocos)
set(GAF_ROOT ${CMAKE_SOURCE_DIR}/../Library)

if(WIN32)
  add_definitions(-D_WINDOWS)
  set(PLATFORM_FOLDER win32)
  set(GAF_PLATFORM_INCLUDE_DIR
    ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/gles/include/OGLES
  )
elseif(MACOSX OR APPLE)
  set(PLATFORM_FOLDER mac)
elseif(IOS)
  add_definitions(-DCC_TARGET_OS_IPHONE)
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
  ${COCOS_ROOT}
  ${COCOS_ROOT}/2d
  ${COCOS_ROOT}/platform/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/zlib/include
  ${COCOS_ROOT}/../external/glfw3/include/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/base
  ${COCOS_ROOT}/math
)

set(CMAKE_EXE_LINKER_FLAGS “-framework AudioToolbox -framework CoreGraphics -framework QuartzCore -framework UIKit -framework OpenGLES”)

file(GLOB_RECURSE gaf_source_files
  "${GAF_ROOT}/Sources/*.cpp")

add_library(gafplayer ${gaf_source_files})

#install .a file
INSTALL(
  TARGETS gafplayer
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  COMPONENT library
)

#install header files
install(DIRECTORY Sources/ DESTINATION "include/GAFPlayer"
  FILES_MATCHING PATTERN "*.h") 
