# SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: MIT

find_package(AppStreamQt REQUIRED)
find_package(Flatpak REQUIRED)

liri_add_plugin(flatpak
    TYPE
        "liri/appcenter/resources"
    SOURCES
        flatpakappstreamjob.cpp
        flatpakappstreamjob.h
        flatpakbackend.cpp
        flatpakbackend.h
        flatpakpluginbase.h
        flatpakplugin.cpp
        flatpakplugin.h
        flatpakresource.cpp
        flatpakresource.h
        flatpaksource.cpp
        flatpaksource.h
        flatpaktransaction.cpp
        flatpaktransaction.h
        flatpaktransactionjob.cpp
        flatpaktransactionjob.h
        flatpakutils.cpp
        flatpakutils.h
        internetcheck.cpp
        internetcheck.h
    DEFINES
        QT_NO_KEYWORD
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Concurrent
        Qt5::Network
        AppStreamQt
        PkgConfig::Flatpak
        Liri::AppCenter
        Liri::AppCenterPrivate
)

install(FILES "io.liri.AppCenter.Flatpak.desktop"
        DESTINATION "${INSTALL_APPLICATIONSDIR}")
