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

# this CMakeLists is to generate GAFPlayer.a

cmake_minimum_required(VERSION 2.6)
project (GAFPlayer)

if(WINDOWS)
  ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
endif()

if(WINDOWS AND NOT BUILD_STATIC)
  ADD_DEFINITIONS (-D_USRDLL)
endif()

set(GAFPLAYER_SRC
  GAFPrecompiled.cpp
  Sources/GAFAnimatedObject.cpp
  Sources/GAFAnimation.cpp
  Sources/GAFAnimationFrame.cpp
  Sources/GAFAnimationSequence.cpp
  Sources/GAFAsset.cpp
  Sources/GAFCachedTexture.cpp
  Sources/GAFFile.cpp
  Sources/GAFFilterData.cpp
  Sources/GAFFilterManager.cpp
  Sources/GAFLoader.cpp
  Sources/GAFQuadCommand.cpp
  Sources/GAFShaderManager.cpp
  Sources/GAFSprite.cpp
  Sources/GAFSpriteWithAlpha.cpp
  Sources/GAFStencilMaskSprite.cpp
  Sources/GAFStream.cpp
  Sources/GAFSubobjectState.cpp
  Sources/GAFTextureAtlas.cpp
  Sources/GAFTextureAtlasElement.cpp
  Sources/PrimitiveDeserializer.cpp
  Sources/TagDefineAnimationFrames.cpp
  Sources/TagDefineAnimationFrames2.cpp
  Sources/TagDefineAnimationMasks.cpp
  Sources/TagDefineAnimationObjects.cpp
  Sources/TagDefineAtlas.cpp
  Sources/TagDefineNamedParts.cpp
  Sources/TagDefineSequences.cpp
  Sources/TagDefineStage.cpp
  Sources/TagDefineTimeline.cpp 
    )

include_directories(
  .
  ./Sources
  ${CMAKE_SOURCE_DIR}/libs/cocos2d-x/cocos/math
  ${CMAKE_SOURCE_DIR}/cocos/math
)

add_library(gafplayer ${GAFPLAYER_SRC})

target_link_libraries(gafplayer cocos2d)

set_target_properties(gafplayer
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
