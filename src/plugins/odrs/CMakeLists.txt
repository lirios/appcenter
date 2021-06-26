if(NOT TARGET Liri::Qt5AccountsService)
    find_package(Qt5AccountsService REQUIRED)
endif()
if(NOT TARGET Liri::LocalDevice)
    find_package(Liri1LocalDevice REQUIRED)
endif()

liri_add_plugin(odrs
    TYPE
        "liri/appcenter/reviews"
    SOURCES
        odrsbackend.cpp
        odrsbackend.h
        odrsplugin.cpp
        odrsplugin.h
        odrsutils.cpp
        odrsutils.h
    PUBLIC_LIBRARIES
        Qt5::Core
        Qt5::Network
        Liri::Qt5AccountsService
        Liri::LocalDevice
        Liri::AppCenter
        Liri::AppCenterPrivate
)
