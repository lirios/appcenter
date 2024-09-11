# Translations
#file(GLOB LiriAppCenter_TRANSLATIONS "${CMAKE_SOURCE_DIR}/translations/app/*_*.ts")
#qt5_add_translation(LiriAppCenter_QM_FILES ${LiriAppCenter_TRANSLATIONS})
#install(FILES ${LiriAppCenter_QM_FILES}
#        DESTINATION "${INSTALL_DATADIR}/liri-appcenter/translations")

qt6_add_executable(LiriAppCenter MANUAL_FINALIZATION main.cpp)
set_target_properties(LiriAppCenter PROPERTIES OUTPUT_NAME "liri-appcenter")
qt6_finalize_target(LiriAppCenter)

target_compile_definitions(LiriAppCenter
    PRIVATE
        APPCENTER_VERSION="${PROJECT_VERSION}"
        QT_NO_CAST_FROM_ASCII
        QT_NO_FOREACH
)

target_link_libraries(LiriAppCenter
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
)

qt6_add_qml_module(LiriAppCenter
    URI io.liri.AppCenter
    VERSION 1.0
    QML_FILES
        qml/AddSourceDialog.qml
        qml/AllAppsTab.qml
        qml/AppPage.qml
        qml/InstalledAppsTab.qml
        qml/Main.qml
        qml/SourcesTab.qml
        qml/UpdatesTab.qml
        qml/app/Buttons.qml
        qml/app/Details.qml
        qml/app/Header.qml
        qml/app/HeaderRatings.qml
        qml/app/Icon.qml
        qml/app/Info.qml
        qml/app/RatingsReviews.qml
        qml/app/Review.qml
        qml/app/Screenshots.qml
        qml/app/Transaction.qml
        qml/pages/AddReviewPage.qml
        qml/pages/ReviewsPage.qml
        qml/ratings/RatingNumber.qml
        qml/ratings/RatingStar.qml
        qml/ratings/StarIcon.qml
        qml/ratings/StarRatings.qml
)

install(TARGETS LiriAppCenter DESTINATION ${KDE_INSTALL_BINDIR})

install(FILES "io.liri.AppCenter.appdata.xml"
        DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES "io.liri.AppCenter.desktop"
        DESTINATION ${KDE_INSTALL_APPDIR})