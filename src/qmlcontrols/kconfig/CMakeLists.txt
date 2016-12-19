project(kconfigplugin)

set(kconfigplugin_SRCS
    kconfigplugin.cpp
    kauthorizedproxy.cpp
    )

add_library(kconfigplugin SHARED ${kconfigplugin_SRCS})
target_link_libraries(kconfigplugin
        Qt5::Core
        Qt5::Qml
        KF5::ConfigCore
)

install(TARGETS kconfigplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

