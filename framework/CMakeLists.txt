include(GenerateExportHeader)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

ecm_setup_version(${PROJECT_VERSION} VARIABLE_PREFIX SOFTWARE
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/software_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/SoftwareConfigVersion.cmake"
                  SOVERSION ${PROJECT_SOVERSION})

include_directories(
    "${CMAKE_CURRENT_BINARY_DIR}/../headers"
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_definitions(-fexceptions)

file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_FILES *.qml)

add_library(LiriSoftware SHARED ${SOURCES})

generate_export_header(LiriSoftware BASE_NAME Software EXPORT_FILE_NAME software/software_export.h)
add_library(Liri::Software ALIAS LiriSoftware)

target_link_libraries(LiriSoftware
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Xml
        Qt5::Concurrent
        Vibe::Core
    PRIVATE
        KF5::Archive
        Flatpak::Flatpak
        Flatpak::Glib
        Flatpak::Gio
        Flatpak::GObject
)

set(SOFTWARE_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/Liri")

target_include_directories(LiriSoftware INTERFACE "$<INSTALL_INTERFACE:${SOFTWARE_INCLUDEDIR}>")

set_target_properties(LiriSoftware PROPERTIES VERSION   ${PROJECT_VERSION}
                                                 SOVERSION ${PROJECT_SOVERSION}
                                                 EXPORT_NAME Software
)

ecm_generate_headers(Software_CamelCase_HEADERS
  HEADER_NAMES
    SoftwareManager Application Source Screenshot
  PREFIX
    Software
  REQUIRED_HEADERS Software_HEADERS
)

install(FILES ${Software_CamelCase_HEADERS}
        DESTINATION ${SOFTWARE_INCLUDEDIR}/Software
        COMPONENT Devel)

install(TARGETS LiriSoftware EXPORT SoftwareTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/software/software_export.h
      ${Software_HEADERS}
    DESTINATION
      ${SOFTWARE_INCLUDEDIR}/software
    COMPONENT
      Devel
)

# Create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/Liri")

ecm_configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/SoftwareConfig.cmake.in"
                                  "${CMAKE_CURRENT_BINARY_DIR}/SoftwareConfig.cmake"
                                  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SoftwareConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/SoftwareConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT SoftwareTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE SoftwareTargets.cmake NAMESPACE Liri::)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/software_version.h
        DESTINATION ${SOFTWARE_INCLUDEDIR}/software COMPONENT Devel)
