if(NOT TARGET Liri::Models)
    find_package(Liri1Models REQUIRED)
endif()

ecm_add_qml_module(AppCenterQmlPlugin
    URI
        "Liri.AppCenter"
    VERSION
        "1.0"
    NO_GENERATE_PLUGIN_SOURCE
    DEPENDENCIES
        "QtQuick"
        "QtQuick.Controls"
        "QtQuick.Layouts"
        "QtQuick.Templates"
        "QtQuick.Controls.Material"
        "Liri.Models"
)

target_sources(AppCenterQmlPlugin
    PRIVATE
        filteredresourcesmodel.cpp filteredresourcesmodel.h
        filteredsourcesmodel.cpp filteredsourcesmodel.h
        filteredreviewsmodel.cpp filteredreviewsmodel.h
        logging.cpp logging.h
        paginatemodel.cpp paginatemodel.h
        plugin.cpp
)

target_link_libraries(AppCenterQmlPlugin
    PUBLIC
        Qt6::Core
        Qt6::DBus
        Qt6::Qml
        Liri::Models
        Liri::AppCenter
)

ecm_finalize_qml_module(AppCenterQmlPlugin)
