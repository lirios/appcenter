file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_SOURCES *.qml)

add_executable(papyros-appstore ${SOURCES})
target_link_libraries(papyros-appstore
                      Qt5::Quick)

install(TARGETS papyros-appstore
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${QML_SOURCES}
DESTINATION ${CMAKE_INSTALL_DATADIR}/papyros-appstore/app)
