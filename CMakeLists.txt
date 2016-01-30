project(io.papyros.AppCenter)

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# Silence CMake warnings
if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()

# Set version
set(PROJECT_VERSION "0.1.0")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_SOVERSION 0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc and rrc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Extra CMake files
find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

# Build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -fvisibility=hidden -fvisibility-inlines-hidden -Werror -Wall -Wextra -Wno-unused-parameter -pedantic -std=c++11")

# Disable debug output for release builds
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Minimum version requirements
set(QT_MIN_VERSION "5.4.0")
set(KF5_MIN_VERSION "5.8.0")

# Find Qt5
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
             Core
             Gui
             Qml
             Quick
             Xml
             Concurrent)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
             Archive)

find_package(XdgApp REQUIRED)
find_package(Papyros REQUIRED)

# Install the desktop and appdata files
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/${PROJECT_NAME}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/data/${PROJECT_NAME}.appdata.xml
        DESTINATION ${CMAKE_INSTALL_METAINFODIR})

add_subdirectory(app)
add_subdirectory(daemon)
add_subdirectory(declarative)
add_subdirectory(framework)

# Display feature summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
