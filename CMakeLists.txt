cmake_minimum_required(VERSION 3.10.0)

project("AppCenter"
    VERSION "0.1.0"
    DESCRIPTION "Software center"
    LANGUAGES CXX C
)

## Shared macros and functions:
if(NOT LIRI_LOCAL_ECM)
    find_package(LiriCMakeShared "2.0.0" REQUIRED NO_MODULE)
    list(APPEND CMAKE_MODULE_PATH "${LCS_MODULE_PATH}")
endif()

## Add some paths to check for CMake modules:
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

## Set minimum versions required.
set(QT_MIN_VERSION "5.10.0")

## Liri specific setup common for all modules:
include(LiriSetup)

## Only build on the appropriate platforms.
if(NOT UNIX OR ANDROID)
    message(NOTICE "Skipping the build as the condition \"UNIX OR ANDROID\" is not met.")
    return()
endif()

## Find Qt 5.
find_package(Qt5 "${QT_MIN_VERSION}"
    CONFIG REQUIRED
    COMPONENTS
        Core
        DBus
        Concurrent
        Gui
        Qml
        Quick
        QuickControls2
        LinguistTools
)

## Treat warnings as errors:
add_compile_options(-Wall -Wextra -Werror)

## Add subdirectories:
add_subdirectory(src/framework)
add_subdirectory(src/app)
add_subdirectory(src/notifier)
add_subdirectory(src/imports/appcenter)
if(LINUX AND NOT ANDROID)
    add_subdirectory(src/plugins/flatpak)
endif()
add_subdirectory(src/plugins/odrs)
