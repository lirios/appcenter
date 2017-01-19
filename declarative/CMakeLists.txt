file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_FILES *.qml)

add_library(lirisoftwareplugin SHARED ${SOURCES})

target_link_libraries(lirisoftwareplugin
                      Qt5::Core
                      Qt5::Qml
                      Vibe::Core
                      Liri::Software)

install(TARGETS lirisoftwareplugin
        DESTINATION ${QML_INSTALL_DIR}/Liri/Software)
install(FILES qmldir ${QML_FILES}
        DESTINATION ${QML_INSTALL_DIR}/Liri/Software)
