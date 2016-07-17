file(GLOB_RECURSE SOURCES *.cpp *.h)

add_executable(papyros-update-notifier ${SOURCES})
target_link_libraries(papyros-update-notifier
                      Qt5::Core
                      Hawaii::Core
                      Papyros::Software
                      KF5::Notifications)

install(TARGETS papyros-update-notifier
        DESTINATION ${CMAKE_INSTALL_BINDIR})
