if(NOT TARGET Liri::Notifications)
    find_package(Liri1Notifications REQUIRED)
endif()

# Translations
file(GLOB LiriAppCenter_TRANSLATIONS "${CMAKE_SOURCE_DIR}/translations/notifier/*_*.ts")
qt6_create_translation(LiriAppCenter_QM_FILES ${CMAKE_SOURCE_DIR} ${LiriAppCenter_TRANSLATIONS})        
install(FILES ${LiriAppCenter_QM_FILES}
        DESTINATION "${KDE_INSTALL_DATADIR}/liri-appcenter/translations")

set(LIBEXECDIR "${KDE_INSTALL_FULL_LIBEXECDIR}")
configure_file(
    "io.liri.AppCenter.Notifier.desktop"
    "${CMAKE_CURRENT_BINARY_DIR}/io.liri.AppCenter.Notifier.desktop"
    @ONLY
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/io.liri.AppCenter.Notifier.desktop"
        DESTINATION "${KDE_INSTALL_AUTOSTARTDIR}")

qt6_add_executable(LiriAppCenterNotifier
    main.cpp
    updatenotifier.cpp updatenotifier.h
    ${LiriAppCenter_QM_FILES}
)

set_target_properties(LiriAppCenterNotifier PROPERTIES
    OUTPUT_NAME liri-appcenter-notifier
)

qt6_finalize_target(LiriAppCenterNotifier)

target_compile_definitions(LiriAppCenterNotifier
    PRIVATE
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
        VERSION="${PROJECT_VERSION}"
)

target_link_libraries(LiriAppCenterNotifier
    PRIVATE
        Qt6::Core
        Liri::AppCenter
        Liri::Notifications
)