# Translations
file(GLOB LiriAppCenter_TRANSLATIONS "${CMAKE_CURRENT_SOURCE_DIR}/translations/*_*.ts")
qt5_add_translation(LiriAppCenter_QM_FILES ${LiriAppCenter_TRANSLATIONS})
install(FILES ${LiriAppCenter_QM_FILES}
        DESTINATION "${INSTALL_DATADIR}/liri-appcenter/translations")

liri_add_executable(LiriAppCenter
    OUTPUT_NAME
        "liri-appcenter"
    SOURCES
        main.cpp
        ${LiriAppCenter_QM_FILES}
    RESOURCES
        appcenter.qrc
    DEFINES
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        APPCENTER_VERSION="${PROJECT_VERSION}"
    APPDATA
        "${CMAKE_CURRENT_SOURCE_DIR}/io.liri.AppCenter.appdata.xml"
    DESKTOP
        "${CMAKE_CURRENT_SOURCE_DIR}/io.liri.AppCenter.desktop"
    LIBRARIES
        Qt5::Core
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        Qt5::QuickControls2
    GUI
)
