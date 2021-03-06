cmake_minimum_required (VERSION 3.1.0)
project (XyGrib)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 1)

set(XYGRIB_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(XYGRIB_VERSION_DATE "2018-06-23")

set(CMAKE_VERBOSE_MAKEFILE ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Unfotunately cmake <3.9 is not able to handle the coding style of XyGrib and AUTOMOC misses many classes. Once the code is fixed or we can expect newer cmake everywhere, we can enable this and get rid of the QT5_WRAP_CPP() macros
## Instruct CMake to run moc automatically when needed
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOMOC_RELAXED_MODE TRUE)

# Unfotunately AUTOUIC_SEARCH_PATHS is not available before cmake 3.9 so we can't use this on all the current platforms
# At some future moment, turn this on and simplify src/GUI/CMakeLists.txt
## Create code from a list of Qt designer ui files
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src/GUI")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
if(UNIX AND NOT APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_PREFIX}/cmake") 

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
include_directories(${Qt5Network_INCLUDE_DIRS})
find_package(Qt5PrintSupport CONFIG REQUIRED)
include_directories(${Qt5PrintSupport_INCLUDE_DIRS})
find_package(Qt5Xml CONFIG REQUIRED)
include_directories(${Qt5Xml_INCLUDE_DIRS})

find_library(LIBNOVA_LIBRARY
     NAMES "libnova.a" "libnova.so"
     PATHS
     $ENV{EXTERNLIBS}/libnova
     ~/Library/Frameworks
     /Library/Frameworks
     /usr/local
     /usr
     /sw
     /opt/local
     /opt/csw
     /opt
     PATH_SUFFIXES lib lib64 lib/x86_64-linux-gnu lib/i386-linux-gnu lib/armhf-linux-gnu lib/arm64-linux-gnu
     REQUIRED
 )
find_path(LIBNOVA_INCLUDE_DIR
     NAMES "libnova/libnova.h"
     PATHS
     $ENV{EXTERNLIBS}/libnova/include
     ~/Library/Frameworks/include
     /Library/Frameworks/include
     /usr/local/include
     /usr/include
     /sw/include # Fink
     /opt/local/include # DarwinPorts
     /opt/csw/include # Blastwave
     /opt/include
     REQUIRED
 )
include_directories(${LIBNOVA_INCLUDE_DIR})

#find_package(OpenJPEG REQUIRED)
find_library(OPENJPEG_LIBRARIES
     NAMES "libopenjp2.a" "libopenjp2.so" "libopenjpeg.so.2"
     PATHS
     $ENV{EXTERNLIBS}/openjpeg
     ~/Library/Frameworks
     /Library/Frameworks
     /usr/local
     /usr
     /sw
     /opt/local
     /opt/csw
     /opt
     PATH_SUFFIXES lib lib64
     REQUIRED
 )
find_path(OPENJPEG_INCLUDE_DIR
     NAMES "openjpeg.h"
     PATHS
     $ENV{EXTERNLIBS}/openjpeg/include
     ~/Library/Frameworks/include
     /Library/Frameworks/include
     /usr/local/include
     /usr/local/include/openjpeg-2.3
     /usr/include/openjpeg-2.3
     /usr/local/include/openjpeg-2.1
     /usr/include/openjpeg-2.1
     /usr/local/include/openjpeg-2.2
     /usr/include/openjpeg-2.2
     /usr/include
     /sw/include # Fink
     /opt/local/include # DarwinPorts
     /opt/csw/include # Blastwave
     /opt/include
     REQUIRED
 )
include_directories(${OPENJPEG_INCLUDE_DIR})

include(cmake/FindPROJ4.cmake)
if(NOT PROJ4_FOUND)
  message(FATAL_ERROR "PROJ.4 library not found!")
endif()
include_directories(${PROJ4_INCLUDE_DIRS})

find_package(BZip2 REQUIRED)
include_directories(${BZIP2_INCLUDE_DIR})

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

configure_file(cmake/Version.h.in ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/include/Version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/include)

if(APPLE)
set(PREFIX_BIN ${PROJECT_NAME})
set(PREFIX_PKGDATA ${PROJECT_NAME})
endif()
if(WIN32)
set(PREFIX_BIN ".")
set(PREFIX_PKGDATA ".")
endif()
if(UNIX AND NOT APPLE)
# This is not exactly a good idea on linux - the code should be changed so that the executable would go to /usr/local/bin and the data to /usr/local/share/XyGrib
set(PREFIX_BIN ${PROJECT_NAME})
set(PREFIX_PKGDATA ${PROJECT_NAME})
endif()

add_subdirectory(src)

# Installation
# macOS bundle parameters
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.in")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE "xyGrib.icns")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_LONG_VERSION_STRING "${XYGRIB_VERSION}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "${XYGRIB_VERSION}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION "${XYGRIB_VERSION}")
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_COPYRIGHT "${PROJECT_NAME} Authors")
# Install data
install(DIRECTORY data DESTINATION ${PREFIX_PKGDATA})
