file(GLOB_RECURSE SOURCES *.cpp *.h papyros-appcenter.qrc)
file(GLOB_RECURSE QML_SOURCES *.qml)

add_executable(papyros-appcenter ${SOURCES})
target_link_libraries(papyros-appcenter
                      Qt5::Quick
                      Papyros::Papyros)

install(TARGETS papyros-appcenter
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${QML_SOURCES}
DESTINATION ${CMAKE_INSTALL_DATADIR}/papyros-appcenter/app)
