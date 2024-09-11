# SPDX-FileCopyrightText: 2024 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

## Enable feature summary at the end of the configure run:
include(FeatureSummary)

## Set minimum versions required:
set(QT_MIN_VERSION "6.7.0")

## Find Qt:
find_package(Qt6 "${QT_MIN_VERSION}"
    REQUIRED
    COMPONENTS
        Core
        Core5Compat
        DBus
        Concurrent
        Gui
        Qml
        QmlIntegration
        Quick
        QuickControls2
        LinguistTools
)

## Silence old policy warnings:
qt6_policy(SET QTP0004 OLD)

## Find AppStreamQt:
find_package(AppStreamQt REQUIRED)

## Find Flatpak:
option(FEATURE_appcenter_flatpak "Flatpak support" ON)
if(FEATURE_appcenter_flatpak)
    find_package(Flatpak QUIET)
    set_package_properties(Flatpak PROPERTIES
        TYPE RECOMMENDED
        PURPOSE "Flatpak support"
    )
    if(NOT TARGET PkgConfig::Flatpak)
        message(WARNING "You need Flatpak for AppCenter::Flatpak")
        set(FEATURE_appcenter_flatpak OFF)
    endif()
endif()
add_feature_info("AppCenter::Flatpak" FEATURE_appcenter_flatpak "Build support for Flatpak")
set(FEATURE_appcenter_flatpak "$<IF:${FEATURE_appcenter_flatpak},1,0>")

#### Features

## Features summary:
if(NOT LIRI_SUPERBUILD)
    feature_summary(WHAT ENABLED_FEATURES DISABLED_FEATURES)
endif()
