 #
 # Copyright (c) 2014 Samsung Electronics Corporation.
 # 
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 # 
 #    http://www.apache.org/licenses/LICENSE-2.0
 # 
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.


# The name of our project is "KV2STREAMER". CMakeLists files in this project can
# refer to the root source directory of the project as ${NEXD_SOURCE_DIR} and
# to the root binary directory of the project as ${NEXD_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (KV2STREAMER)

# The version number.
set (KV2STREAMER_VERSION_MAJOR 0)
set (KV2STREAMER_VERSION_MINOR 3)
set (KV2STREAMER_VERSION_PATCH 0)
set (KV2STREAMER_VERSION
     ${KV2STREAMER_VERSION_MAJOR}.${KV2STREAMER_VERSION_MINOR}.${KV2STREAMER_VERSION_PATCH})

# DEBUG:
set (CMAKE_CXX_FLAGS "-Wreturn-type")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/tool/kv2client-config.h.in"
  "${PROJECT_BINARY_DIR}/tool/kv2client-config.h"
  )
  
include_directories ("${PROJECT_SOURCE_DIR}/")

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake) 

# OSCPACK - must be the version with mutlicast
set (OSCPACK_INCLUDE_DIRS 
    "/usr/local/include/oscpack"
    CACHE STRING "OSCPACK include path")
set (OSCPACK_LIBRARY_PATH
    "/usr/local/lib"
    CACHE STRING "OSCPACK library path")

# BOOST
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.55.0 COMPONENTS system filesystem log thread program_options date_time) 
    
if(Boost_FOUND) 
  include_directories(${Boost_INCLUDE_DIRS}) 
  set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})
endif()

# GSTREAMER
find_package (GStreamer COMPONENTS gstreamer-app gstreamer-video)
find_package (GLIB COMPONENTS gio gobject gmodule gthread)

include_directories (${GSTREAMER_INCLUDE_DIRS})
include_directories (${GLIB_INCLUDE_DIRS})
include_directories (${GSTREAMER_APP_INCLUDE_DIRS})
include_directories (${GSTREAMER_VIDEO_INCLUDE_DIRS})

set (EXTRA_LIBS ${EXTRA_LIBS} ${GLIB_LIBRARIES} ${GLIB_GOBJECT_LIBRARIES}
     ${GLIB_GMODULE_LIBRARIES} ${GLIB_GTHREAD_LIBRARIES}
     ${GSTREAMER_LIBRARIES}
     ${GSTREAMER_APP_LIBRARIES}
     ${GSTREAMER_VIDEO_LIBRARIES}
     )

# Recurse into the subdirectories. This does not actually cause another cmake
# executable to run. The same process will walk through the project's entire
# directory structure.
add_subdirectory (kv2client)
add_subdirectory (oscpack-wrapper)
add_subdirectory (gst-wrapper)
add_subdirectory (tool)

# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE  
     "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${KV2STREAMER_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${KV2STREAMER_VERSION_MINOR}")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "maintainer@test.com")
# TODO: add pacakge dependencies
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12), python2.6, libboost-program-options1.40.0 (>= 1.40.0)")
set(CPACK_SET_DESTDIR "ON")

include (CPack)
