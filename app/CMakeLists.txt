cmake_minimum_required(VERSION 3.4.1)

if (${ANDROID_ABI} STREQUAL "x86_64")
include( ${CMAKE_CURRENT_SOURCE_DIR}/libs/build/alox/ALox.cmake )
endif()

#add_library(ipcamera-sdk SHARED IMPORTED)

#set_target_properties(
#                    ipcamera-sdk
#                    PROPERTIES IMPORTED_LOCATION
#                    libs/${ANDROID_ABI}/libNetDEVSDK.so
#                    )

#add_library(
#             ipcamera
#             SHARED

#             src/main/cpp/ipcamera/IPCameraJNI.cpp
#             src/main/cpp/ipcamera/IPCamera.cpp
#             )

add_library(
              tennisbase
              SHARED

              src/main/cpp/tennisbase/FileGrabber.cpp
              src/main/cpp/tennisbase/FileItem.cpp
              src/main/cpp/tennisbase/FileTracker.cpp

              src/main/cpp/util/inih/ini.c
              src/main/cpp/util/inih/INIReader.cpp
              )

add_library(
             tennisbout
             SHARED

             src/main/cpp/tennisbout/TennisBoutJNIImpl.cpp
             src/main/cpp/tennisbout/TennisBoutJNI.cpp
             src/main/cpp/tennisbout/TennisBout.cpp

             src/main/cpp/util/ALoxHelper.cpp
             src/main/cpp/util/TennisBaseJNI.cpp
             src/main/cpp/util/TennisBaseHelper.cpp
             )

add_library(
             tennisdrill
             SHARED

             src/main/cpp/tennisdrill/datas/HandleDatasHand.cpp
             src/main/cpp/tennisdrill/datas/HandleDatasReturnServe.cpp
             src/main/cpp/tennisdrill/datas/HandleDatasServe.cpp
             src/main/cpp/tennisdrill/datas/HandleDatasVolley.cpp
             src/main/cpp/tennisdrill/datas/HandleDatas.cpp

             src/main/cpp/tennisdrill/stat/HandleStatHand.cpp
             src/main/cpp/tennisdrill/stat/HandleStatReturnServe.cpp
             src/main/cpp/tennisdrill/stat/HandleStatServe.cpp
             src/main/cpp/tennisdrill/stat/HandleStatVolley.cpp
             src/main/cpp/tennisdrill/stat/HandleStat.cpp

             src/main/cpp/tennisdrill/HandleProxy.cpp
             src/main/cpp/tennisdrill/TennisDrill.cpp
             src/main/cpp/tennisdrill/TennisDrillJNI.cpp
             src/main/cpp/tennisdrill/TennisDrillJNIImpl.cpp

             src/main/cpp/util/ALoxHelper.cpp
             src/main/cpp/util/TennisBaseJNI.cpp
             src/main/cpp/util/TennisBaseHelper.cpp
)

include_directories(
            src/main/cpp/interface
            src/main/cpp/util
            src/main/cpp/util/alox
            )

find_library( log-lib log )

set( TENNIS_BASE_COMPILER_SYMBOLS ${TENNIS_BASE_COMPILER_SYMBOLS} "__TENNIS_BASE_MODULE__" )
set( TENNIS_BASE_COMPILER_SYMBOLS ${TENNIS_BASE_COMPILER_SYMBOLS} "__TENNIS_BASE_TEST__" )
if (NOT ${ANDROID_ABI} STREQUAL "x86_64")
set( TENNIS_BASE_COMPILER_SYMBOLS ${TENNIS_BASE_COMPILER_SYMBOLS} "__ANDROID_LOG_OFF__" )
endif()
target_compile_definitions( tennisbase PUBLIC ${TENNIS_BASE_COMPILER_SYMBOLS} )

set( TENNIS_BOUT_COMPILER_SYMBOLS ${TENNIS_BOUT_COMPILER_SYMBOLS} "__TENNIS_BOUT_MODULE__" )
if (NOT ${ANDROID_ABI} STREQUAL "x86_64")
set( TENNIS_BOUT_COMPILER_SYMBOLS ${TENNIS_BOUT_COMPILER_SYMBOLS} "__ANDROID_LOG_OFF__" )
endif()
target_compile_definitions( tennisbout PUBLIC ${TENNIS_BOUT_COMPILER_SYMBOLS} )

set( TENNIS_DRILL_COMPILER_SYMBOLS ${TENNIS_DRILL_COMPILER_SYMBOLS} "__TENNIS_DRILL_MODULE__" )
set( TENNIS_DRILL_COMPILER_SYMBOLS ${TENNIS_DRILL_COMPILER_SYMBOLS} "__TENNIS_DRILL_TEST__" )
if (NOT ${ANDROID_ABI} STREQUAL "x86_64")
set( TENNIS_DRILL_COMPILER_SYMBOLS ${TENNIS_DRILL_COMPILER_SYMBOLS} "__ANDROID_LOG_OFF__" )
endif()
target_compile_definitions( tennisdrill PUBLIC ${TENNIS_DRILL_COMPILER_SYMBOLS} )

#set( TENNIS_IPCAMERA_COMPILER_SYMBOLS ${TENNIS_IPCAMERA_COMPILER_SYMBOLS} "__TENNIS_IPCAMERA_MODULE__" )
#if (NOT ${ANDROID_ABI} STREQUAL "x86_64")
#set( TENNIS_IPCAMERA_COMPILER_SYMBOLS ${TENNIS_IPCAMERA_COMPILER_SYMBOLS} "__ANDROID_LOG_OFF__" )
#endif()
#target_compile_definitions( ipcamera PUBLIC ${TENNIS_IPCAMERA_COMPILER_SYMBOLS} )

#target_link_libraries( ipcamera ipcamera-sdk ${log-lib} )

if (${ANDROID_ABI} STREQUAL "x86_64")
target_link_libraries( tennisbase alox ${log-lib} )
target_link_libraries( tennisbout alox ${log-lib} )
target_link_libraries( tennisdrill alox ${log-lib} )
else()
target_link_libraries( tennisbase ${log-lib} )
target_link_libraries( tennisbout ${log-lib} )
target_link_libraries( tennisdrill ${log-lib} )
endif()