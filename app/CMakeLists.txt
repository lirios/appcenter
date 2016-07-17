file(GLOB_RECURSE SOURCES *.cpp *.h papyros-appcenter.qrc)

add_executable(papyros-appcenter ${SOURCES})
target_link_libraries(papyros-appcenter
                      Qt5::Quick
                      KF5::Archive
                      Hawaii::Core)

install(TARGETS papyros-appcenter
        DESTINATION ${CMAKE_INSTALL_BINDIR})
