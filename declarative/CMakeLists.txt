file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_FILES *.qml)

add_library(papyrossoftware SHARED ${SOURCES})

target_link_libraries(papyrossoftware
                      Qt5::Core
                      Qt5::Qml
                      Papyros::Papyros
                      Papyros::Software)

install(TARGETS papyrossoftware
        DESTINATION ${QML_INSTALL_DIR}/Papyros/Software)
install(FILES qmldir ${QML_FILES}
        DESTINATION ${QML_INSTALL_DIR}/Papyros/Software)
