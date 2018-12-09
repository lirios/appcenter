if(NOT TARGET Liri::Notifications)
    find_package(Liri1Notifications REQUIRED)
endif()

liri_add_executable(LiriUpdateNotifier
    OUTPUT_NAME
        "liri-update-notifier"
    SOURCES
        main.cpp
        updatenotifier.cpp
        updatenotifier.h
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        APPCENTER_VERSION="${PROJECT_VERSION}"
    LIBRARIES
        Qt5::Core
        Liri::AppCenter
        Liri::Notifications
    GUI
)
