find_package(AppStreamQt REQUIRED)
find_package(Flatpak REQUIRED)

liri_add_plugin(flatpak
    TYPE
        "liri/appcenter"
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
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Concurrent
        Qt5::Network
        AppStreamQt
        PkgConfig::Flatpak
        Liri::AppCenter
)

install(FILES "io.liri.AppCenter.Flatpak.desktop"
        DESTINATION "${INSTALL_APPLICATIONSDIR}")
