cmake_minimum_required(VERSION 3.19)

project("AppCenter"
    VERSION "0.1.0"
    DESCRIPTION "Software center"
    LANGUAGES CXX C
)

## Only build on the appropriate platforms.
if(NOT UNIX OR ANDROID)
    message(NOTICE "\"${CMAKE_PROJECT_NAME}\" can be build only on UNIX-like systems, Android excluded")
    return()
endif()

## Add some paths to check for CMake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## ECM:
find_package(ECM 5.245.0 REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

## Installation directories:
include(KDEInstallDirs)

## Compiler settings:
set(KDE_SKIP_NULLPTR_WARNINGS_SETTINGS TRUE)
include(KDECompilerSettings NO_POLICY_SCOPE)

## CMake settings:
include(KDECMakeSettings)

## QML module:
include(ECMQmlModule)

## Disable use of C++ API deprecated in Qt 5.15
add_compile_definitions(QT_DISABLE_DEPRECATED_UP_TO=0x050F00)

## Enable C++17:
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Treat warnings as errors:
add_compile_options(-Wall -Wextra -Werror)

## Shared macros and functions:
if(NOT LIRI_LOCAL_ECM)
    find_package(LiriCMakeShared "2.0.99" REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH "${LCS_MODULE_PATH}")
endif()

## Liri specific setup common for all modules:
include(LiriSetup)

## Features:
include(features.cmake)

## QML import path:
if(NOT QML_IMPORT_PATH)
    set(QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/bin CACHE STRING "" FORCE)
endif()

## Add subdirectories:
add_subdirectory(src/framework)
add_subdirectory(src/app)
add_subdirectory(src/notifier)
add_subdirectory(src/imports/appcenter)
if(LINUX AND NOT ANDROID AND TARGET PkgConfig::Flatpak)
    add_subdirectory(src/plugins/flatpak)
endif()
add_subdirectory(src/plugins/odrs)
