cmake_minimum_required(VERSION 3.19)

# Adjust these parameters for your hardware platform
# must be set as cache strings to ensure they are not overwritten in mesh SDK
set(IC "nRF52840" CACHE STRING "IC")
set(PLATFORM "nrf52840_xxAA"  CACHE STRING "Platform")
#set(BOARD "SEEED_XIAO_SENSE" CACHE STRING "Board type")
set(BOARD "JPOWER_V1" CACHE STRING "Board type")
set(SOFTDEVICE_TYPE "s340" CACHE STRING "SoftDevice type")
set(SOFTDEVICE_VERSION "6.1.1" CACHE STRING "SoftDevice version")

# enable to output verbose info
set(CMAKE_VERBOSE_MAKEFILE off)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_BUGFIX 0)
set(VERSION_SUFFIX "")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}${VERSION_SUFFIX}" CACHE STRING "" FORCE)

set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR} CACHE STRING "" FORCE)

# Modify this to set the correct path relative to this file.
set(nRF5_cmake_path "${CMAKE_SOURCE_DIR}/nRF5-cmake-scripts/nRF5-cmake.cmake")
if(NOT EXISTS "${nRF5_cmake_path}")
    set(nRF5_cmake_path "${CMAKE_SOURCE_DIR}/../nRF5-cmake.cmake")
endif()
include("${nRF5_cmake_path}")

set(jpowlib_cmake_path "${CMAKE_SOURCE_DIR}/cmake/jpowlib.cmake")
include("${jpowlib_cmake_path}")

project(JPower
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}
        LANGUAGES NONE)

message(STATUS "Configuring CMake for nRF5 SDK for ${PROJECT_NAME} ${VERSION_STRING}")
nRF5_setup()

if(NOT TARGET download)
    add_subdirectory("src")
endif()