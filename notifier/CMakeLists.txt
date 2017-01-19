file(GLOB_RECURSE SOURCES *.cpp *.h)

add_executable(liri-update-notifier ${SOURCES})
target_link_libraries(liri-update-notifier
                      Qt5::Core
                      Vibe::Core
                      Liri::Software)

install(TARGETS liri-update-notifier
        DESTINATION ${CMAKE_INSTALL_BINDIR})
