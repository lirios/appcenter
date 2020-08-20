# SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: MIT

if(NOT TARGET Liri::Notifications)
    find_package(Liri1Notifications REQUIRED)
endif()

# Translations
file(GLOB LiriAppCenter_TRANSLATIONS "${CMAKE_SOURCE_DIR}/translations/notifier/*_*.ts")
qt5_add_translation(LiriAppCenter_QM_FILES ${LiriAppCenter_TRANSLATIONS})
install(FILES ${LiriAppCenter_QM_FILES}
        DESTINATION "${INSTALL_DATADIR}/liri-appcenter/translations")

if(IS_ABSOLUTE "${INSTALL_LIBEXECDIR}")
    set(LIBEXECDIR "${INSTALL_LIBEXECDIR}")
else()
    set(LIBEXECDIR "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIBEXECDIR}")
endif()
configure_file(
    "io.liri.AppCenter.Notifier.desktop"
    "${CMAKE_CURRENT_BINARY_DIR}/io.liri.AppCenter.Notifier.desktop"
    @ONLY
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/io.liri.AppCenter.Notifier.desktop"
    DESTINATION
        "${INSTALL_SYSCONFDIR}/xdg/autostart"
)

liri_add_executable(LiriAppCenterNotifier
    OUTPUT_NAME
        "liri-appcenter-notifier"
    INSTALL_DIRECTORY
        "${INSTALL_LIBEXECDIR}"
    SOURCES
        main.cpp
        updatenotifier.cpp
        updatenotifier.h
        ${LiriAppCenter_QM_FILES}
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        VERSION="${PROJECT_VERSION}"
    DESKTOP
        "${CMAKE_CURRENT_BINARY_DIR}/io.liri.AppCenter.Notifier.desktop"
    LIBRARIES
        Qt5::Core
        Liri::AppCenter
        Liri::Notifications
    GUI
)
