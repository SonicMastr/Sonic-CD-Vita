cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(PLATFORM "vita" CACHE STRING "Platform to build for")
set_property(CACHE PLATFORM PROPERTY STRINGS pc vita)

IF(PLATFORM STREQUAL "pc")
ELSEIF(PLATFORM STREQUAL "vita")
    IF(DEFINED ENV{DOLCESDK})
        include("$ENV{DOLCESDK}/share/dolce.toolchain.cmake" REQUIRED)
        include("$ENV{DOLCESDK}/share/dolce.cmake" REQUIRED)
        set(DOLCE_APP_NAME "Sonic CD")
        set(DOLCE_TITLEID  "SOCD00001")
        set(DOLCE_VERSION  "01.00")
        set(DOLCE_MKSFOEX_FLAGS "${DOLCE_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")
        set(DOLCE_ELF_CREATE_FLAGS "${DOLCE_ELF_CREATE_FLAGS} -h 4194304")
    ELSE()
        message(FATAL_ERROR "Please define DOLCESDK to point to your SDK path!")
    ENDIF()
ELSE()
    message(FATAL_ERROR "Wrong platform")
ENDIF()

project(soniccd CXX C)

set (soniccd_VERSION_MAJOR 1)
set (soniccd_VERSION_MINOR 0)
set (soniccd_VERSION_RELEASE 0)

include(CheckCXXCompilerFlag)
include(GNUInstallDirs)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${soniccd_SOURCE_DIR})
include_directories("${CMAKE_SOURCE_DIR}/dependencies/all/theoraplay/")

file(GLOB_RECURSE sonic_SRC
    SonicCDDecomp/*.cpp
)

SET(SOURCES
    ${sonic_SRC}
    dependencies/all/theoraplay/theoraplay.c
)

add_definitions("-Wall")
add_definitions("-std=c++11")

add_executable(soniccd ${SOURCES})

IF(PLATFORM STREQUAL "pc")
    target_link_libraries(soniccd SDL2::SDL2 theoradec theora vorbisfile vorbis ogg pthread)

ELSEIF(PLATFORM STREQUAL "vita")
    add_definitions("-D__VITA__")
    target_link_libraries(soniccd SDL2::SDL2-static
      theoradec
      theora
      vorbisfile
      vorbis
      ogg
      pthread
    )

  dolce_create_self(${PROJECT_NAME}.self soniccd UNSAFE)
  dolce_create_vpk(${PROJECT_NAME}.vpk ${DOLCE_TITLEID} ${PROJECT_NAME}.self
    VERSION ${DOLCE_VERSION}
    NAME ${DOLCE_APP_NAME}
    FILE sce_sys sce_sys
  )
ENDIF()
