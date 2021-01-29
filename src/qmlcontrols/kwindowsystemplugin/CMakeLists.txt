project(kwindowsystemplugin)

set(kwindowsystemplugin_SRCS
    kwindowsystemplugin.cpp
    kwindowsystemproxy.cpp
    )

add_library(kwindowsystem SHARED ${kwindowsystemplugin_SRCS})
target_link_libraries(kwindowsystem
        Qt5::Core
        Qt5::Qml
        KF5::WindowSystem
)

install(TARGETS kwindowsystem DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)
