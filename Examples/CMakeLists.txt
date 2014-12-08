cmake_minimum_required(VERSION 2.6)

cmake_policy(SET CMP0011 NEW)

set(APP_NAME GAFExample)
project (${APP_NAME})

if(WIN32)
  ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
  set(PLATFORM_FOLDER win32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  ADD_DEFINITIONS(-DLINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER android)
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()

# Set paths for gaf library
set(COCOS_ROOT ${CMAKE_SOURCE_DIR}/../../cocos2d-x/cocos)
set(GAF_ROOT ${CMAKE_SOURCE_DIR}/../Library)
include(../Library/CMakeLists.txt)

if(WIN32)
  # set(PLATFORM_SOURCES )
elseif(MACOSX OR APPLE)
  # set(PLATFORM_SOURCES )
elseif(LINUX)
  set(PLATFORM_SOURCES 
  Example.linux/main.cpp
  )
elseif(ANDROID)
else()
endif()

set(GAME_SRC  
  Classes/AppDelegate.cpp
  Classes/GafFeatures.cpp
)

include_directories(
  Classes
  ../Library/Sources
)

# add the executable
add_executable(${APP_NAME}
  ${GAME_SRC}

)

set(FMOD_LIB "fmodex")

  set(ARCH_DIR "32-bit")
  set(PLATFORM_FOLDER_ARCH
      ${PLATFORM_FOLDER}/${ARCH_DIR})


  link_directories(
    ${PLATFORM_LINK_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/tinyxml2/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/jpeg/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/tiff/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/webp/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/png/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/freetype2/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/websockets/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/chipmunk/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/box2d/prebuilt/${PLATFORM_FOLDER_ARCH}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos2d-x/external/curl/prebuilt/${PLATFORM_FOLDER_ARCH}

  )

target_link_libraries(${APP_NAME}
  lib/websockets
  lib/freetype250
  lib/glew32
  lib/glfw3
  lib/libbox2d
  lib/libchipmunk
  lib/libcocos2d
  lib/libcurl_imp
  lib/libiconv
  lib/libjpeg
  lib/libmpg123
  lib/libogg
  lib/libpng
  lib/libSpine
  lib/libtiff
  lib/libvorbis
  lib/libvorbisfile
  lib/libwebp
  lib/libzlib
  lib/OpenAL32
  lib/sqlite3
  gafplayer
  )

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}"
     )


add_custom_command(TARGET ${APP_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
  )

