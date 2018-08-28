cmake_minimum_required(VERSION 3.1)

project(KDHockeyApp LANGUAGES C CXX)

option(KDHOCKEYAPP_QMLSUPPORT_ENABLED "Build code that supports QML applications" ON)
option(KDHOCKEYAPP_SKIP_EXAMPLES "Do not build example code")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    find_package(Qt5 COMPONENTS AndroidExtras REQUIRED)
endif()

if(KDHOCKEYAPP_QMLSUPPORT_ENABLED)
    find_package(Qt5 COMPONENTS Qml REQUIRED)
endif()

if(NOT KDHOCKEYAPP_SKIP_EXAMPLES)
    find_package(Qt5 COMPONENTS QuickWidgets REQUIRED)
    add_subdirectory(examples)
endif()

add_subdirectory(src)
