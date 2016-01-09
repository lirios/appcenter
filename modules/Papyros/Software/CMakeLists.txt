file(GLOB_RECURSE SOURCES *.cpp *.h)
file(GLOB_RECURSE QML_FILES *.qml)

add_library(papyrossoftware SHARED ${SOURCES})

target_link_libraries(papyrossoftware
                      Qt5::Core
                      Qt5::Qml
                      Qt5::Xml
                      Papyros::Papyros
                      ${XdgApp_LIBRARIES})

include_directories(${XdgApp_INCLUDE_DIRS})

install(FILES qmldir ${QML_FILES}
    DESTINATION ${QML_INSTALL_DIR}/Papyros/Software)
install(TARGETS papyrossoftware
    DESTINATION ${QML_INSTALL_DIR}/Papyros/Software)
