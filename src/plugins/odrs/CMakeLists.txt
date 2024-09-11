if(NOT TARGET Liri::Qt6AccountsService)
    find_package(Qt6AccountsService REQUIRED)
endif()
if(NOT TARGET Liri::LocalDevice)
    find_package(Liri1LocalDevice REQUIRED)
endif()

qt6_add_plugin(AppCenterOdrsPlugin
    CLASS_NAME OdrsPlugin
    MANUAL_FINALIZATION
    odrsbackend.cpp odrsbackend.h
    odrsplugin.cpp odrsplugin.h
    odrsutils.cpp odrsutils.h
)

set_target_properties(AppCenterOdrsPlugin PROPERTIES
    OUTPUT_NAME odrs
)

qt6_finalize_target(AppCenterOdrsPlugin)

target_link_libraries(AppCenterOdrsPlugin
    PRIVATE
        Qt6::Core
        Qt6::Network
        Liri::Qt6AccountsService
        Liri::LocalDevice
        Liri::AppCenter
        Liri::AppCenterPrivate
)

install(TARGETS AppCenterOdrsPlugin
        DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/liri/appcenter/reviews")
