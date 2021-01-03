cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{DOLCESDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{DOLCESDK}/share/dolce.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
  endif()
endif()

project(GLES20_Cube)
include("${DOLCESDK}/share/dolce.cmake" REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast -Wall -fno-lto -fno-pic -fno-plt")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++11")
set(DOLCE_MKSFOEX_FLAGS "${DOLCE_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

project(SonicCD-Vita)

include_directories(
    "SonicCDDecomp"
    "dependencies/all/theoraplay"
)

# Build self

set(ELF "sonic.elf")
set(SELF "sonic.self")

file(GLOB_RECURSE sonic_SRC
    SonicCDDecomp/*.cpp
)

add_executable("${ELF}"
    ${sonic_SRC}
    dependencies/all/theoraplay/theoraplay.c
)

target_link_libraries("${ELF}"
  SDL2
  SDL2_image
  pib
  vorbisfile
  vorbis
  ogg
  mpg123
  FLAC
  mikmod
  opus
  freetype
  jpeg
  png
  theora
  SceMotion_stub
  SceCommonDialog_stub
  SceHid_stub
  SceGxm_stub
  SceAudio_stub
  ScePower_stub
  SceDisplayUser_stub
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  SceIofilemgr_stub
  SceSsl_stub
  SceSysmodule_stub
  pthread
  ${SDL2_LIBRARIES}
  gcc
  g
  m
)

dolce_create_self("${SELF}"
  "${ELF}"
)

# Build VPK

set(VPK "SonicCD.vpk")
set(TITLE_NAME "Sonic CD")
set(TITLE_ID "SOCD00001")
set(TITLE_VER "01.00")

dolce_create_vpk("${VPK}" "${TITLE_ID}" "${SELF}"
  NAME "${TITLE_NAME}"
  VERSION "${TITLE_VER}"
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
	FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
	FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
	FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)