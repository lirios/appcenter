if(NOT TARGET Liri::Models)
    find_package(Liri1Models REQUIRED)
endif()

liri_add_qml_plugin(AppCenterQmlPlugin
    MODULE_PATH
        "Liri/AppCenter"
    OUTPUT_NAME
        "liriappcenterplugin"
    VERSION
        "1.0"
    SOURCES
        filteredresourcesmodel.cpp
        filteredresourcesmodel.h
        filteredsourcesmodel.cpp
        filteredsourcesmodel.h
        filteredreviewsmodel.cpp
        filteredreviewsmodel.h
        logging.cpp
        logging.h
        paginatemodel.cpp
        paginatemodel.h
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

liri_finalize_qml_plugin(AppCenterQmlPlugin)
