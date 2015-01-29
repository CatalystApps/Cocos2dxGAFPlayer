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


if(WIN32)
set(GAF_PLATFORM_INCLUDE_DIR
  ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/gles/include/OGLES
  )
elseif(MACOSX OR APPLE)
elseif(LINUX)
elseif(ANDROID)
else()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder")

if(IOS OR MACOX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()


set(GAFPLAYER_SRC
  ${GAF_ROOT}/Sources/GAFAnimationFrame.cpp		
  ${GAF_ROOT}/Sources/GAFFile.cpp			
  ${GAF_ROOT}/Sources/GAFMovieClip.cpp		
  ${GAF_ROOT}/Sources/GAFSprite.cpp			
  ${GAF_ROOT}/Sources/GAFTextureAtlas.cpp		
  ${GAF_ROOT}/Sources/TagDefineAnimationFrames.cpp	
  ${GAF_ROOT}/Sources/TagDefineNamedParts.cpp
  ${GAF_ROOT}/Sources/GAFAnimationSequence.cpp	
  ${GAF_ROOT}/Sources/GAFFilterData.cpp		
  ${GAF_ROOT}/Sources/GAFObject.cpp			
  ${GAF_ROOT}/Sources/GAFStream.cpp			
  ${GAF_ROOT}/Sources/GAFTextureAtlasElement.cpp	
  ${GAF_ROOT}/Sources/TagDefineAnimationFrames2.cpp	
  ${GAF_ROOT}/Sources/TagDefineSequences.cpp
  ${GAF_ROOT}/Sources/GAFAsset.cpp			
  ${GAF_ROOT}/Sources/GAFFilterManager.cpp		
  ${GAF_ROOT}/Sources/GAFPrecompiled.cpp		
  ${GAF_ROOT}/Sources/GAFSubobjectState.cpp		
  ${GAF_ROOT}/Sources/GAFTimeline.cpp			
  ${GAF_ROOT}/Sources/TagDefineAnimationMasks.cpp	
  ${GAF_ROOT}/Sources/TagDefineStage.cpp
  ${GAF_ROOT}/Sources/GAFAssetTextureManager.cpp	
  ${GAF_ROOT}/Sources/GAFLoader.cpp			
  ${GAF_ROOT}/Sources/GAFQuadCommand.cpp		
  ${GAF_ROOT}/Sources/GAFTextData.cpp			
  ${GAF_ROOT}/Sources/GAFTimelineAction.cpp		
  ${GAF_ROOT}/Sources/TagDefineAnimationObjects.cpp	
  ${GAF_ROOT}/Sources/TagDefineTextField.cpp
  ${GAF_ROOT}/Sources/GAFCachedTexture.cpp		
  ${GAF_ROOT}/Sources/GAFMask.cpp			
  ${GAF_ROOT}/Sources/GAFShaderManager.cpp		
  ${GAF_ROOT}/Sources/GAFTextField.cpp		
  ${GAF_ROOT}/Sources/PrimitiveDeserializer.cpp	
  ${GAF_ROOT}/Sources/TagDefineAtlas.cpp		
  ${GAF_ROOT}/Sources/TagDefineTimeline.cpp
    )


include_directories(
  ${GAF_PLATFORM_INCLUDE_DIR}
  ${GAF_ROOT}
  ${GAF_ROOT}/Sources
  ${COCOS_ROOT}
  ${COCOS_ROOT}/2d
  ${COCOS_ROOT}/platform/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/../external/${PLATFORM_FOLDER}-specific/zlib/include
  ${COCOS_ROOT}/../external/glfw3/include/${PLATFORM_FOLDER}
  ${COCOS_ROOT}/math
)

add_library(gafplayer STATIC ${GAFPLAYER_SRC})

set_target_properties(gafplayer
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

