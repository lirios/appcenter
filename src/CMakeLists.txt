file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_SOURCES *.qml)

add_executable(papyros-software ${SOURCES})
target_link_libraries(papyros-software
                      Qt5::Quick)

install(TARGETS papyros-software
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${QML_SOURCES}
DESTINATION ${CMAKE_INSTALL_DATADIR}/papyros-software/app)
