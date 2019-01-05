if(NOT TARGET Liri::Notifications)
    find_package(Liri1Notifications REQUIRED)
endif()

# Translations
file(GLOB LiriAppCenter_TRANSLATIONS "${CMAKE_CURRENT_SOURCE_DIR}/translations/*_*.ts")
qt5_add_translation(LiriAppCenter_QM_FILES ${LiriAppCenter_TRANSLATIONS})
install(FILES ${LiriAppCenter_QM_FILES}
        DESTINATION "${INSTALL_DATADIR}/liri-appcenter/translations")

liri_add_executable(LiriUpdateNotifier
    OUTPUT_NAME
        "liri-update-notifier"
    SOURCES
        main.cpp
        updatenotifier.cpp
        updatenotifier.h
        ${LiriAppCenter_QM_FILES}
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
