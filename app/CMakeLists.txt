file(GLOB_RECURSE SOURCES *.cpp *.h appcenter.qrc)

add_executable(liri-appcenter ${SOURCES})
target_link_libraries(liri-appcenter
                      Qt5::Quick
                      Qt5::QuickControls2
                      KF5::Archive
                      Vibe::Core)

install(TARGETS liri-appcenter
        DESTINATION ${CMAKE_INSTALL_BINDIR})
