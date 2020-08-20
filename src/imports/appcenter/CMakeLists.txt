# SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
#
# SPDX-License-Identifier: MIT

if(NOT TARGET Liri::Models)
    find_package(Liri1Models REQUIRED)
endif()

liri_add_qml_plugin(liriappcenter
    MODULE_PATH
        "Liri/AppCenter"
    VERSION
        "1.0"
    SOURCES
        filteredresourcesmodel.cpp
        filteredresourcesmodel.h
        logging.cpp
        logging.h
        plugin.cpp
    QML_FILES
        qmldir
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
    PUBLIC_LIBRARIES
        Liri::Models
        Liri::AppCenter
)
