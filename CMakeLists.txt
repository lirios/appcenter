project(io.papyros.AppCenter)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Werror -Wall -Wextra -Wno-unused-parameter -pedantic -std=c++11")

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

add_subdirectory(src)
add_subdirectory(modules/Papyros/Software)

# Display feature summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
