qt6_add_plugin(AppCenterFlatpakPlugin
    CLASS_NAME FlatpakPlugin
    MANUAL_FINALIZATION
    flatpakappstreamjob.cpp flatpakappstreamjob.h
    flatpakbackend.cpp flatpakbackend.h
    flatpakpluginbase.h flatpakplugin.cpp flatpakplugin.h
    flatpakresource.cpp flatpakresource.h
    flatpaksource.cpp flatpaksource.h
    flatpaktransaction.cpp flatpaktransaction.h
    flatpaktransactionjob.cpp flatpaktransactionjob.h
    flatpakutils.cpp flatpakutils.h
    internetcheck.cpp internetcheck.h
)

set_target_properties(AppCenterFlatpakPlugin PROPERTIES
    OUTPUT_NAME flatpak
)

qt6_finalize_target(AppCenterFlatpakPlugin)

target_compile_definitions(AppCenterFlatpakPlugin
    PRIVATE
        QT_NO_CAST_FROM_ASCII
        QT_NO_URL_CAST_FROM_STRING
        QT_NO_FOREACH
        QT_NO_KEYWORDS
        QT_NO_SIGNALS_SLOTS_KEYWORDS
)

if(Flatpak_VERSION VERSION_LESS 1.1.2)
    target_compile_definitions(AppCenterFlatpakPlugin
        PRIVATE
            FLATPAK_EXTERNC_REQUIRED
    )
else()
    target_compile_definitions(AppCenterFlatpakPlugin
        PRIVATE
            FLATPAK_VERBOSE_PROGRESS
            FLATPAK_LIST_UNUSED_REFS
    )
endif()

target_link_libraries(AppCenterFlatpakPlugin
    PRIVATE
        Qt6::Core
        Qt6::Concurrent
        Qt6::Network
        AppStreamQt
        PkgConfig::Flatpak
        Liri::AppCenter
        Liri::AppCenterPrivate
)

install(TARGETS AppCenterFlatpakPlugin
        DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/liri/appcenter/resources")

install(FILES "io.liri.AppCenter.Flatpak.desktop"
        DESTINATION "${KDE_INSTALL_APPDIR}")
