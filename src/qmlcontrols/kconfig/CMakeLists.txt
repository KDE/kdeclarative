project(kconfigplugin)

set(kconfigplugin_SRCS
    kauthorizedproxy.cpp
    kauthorizedproxy.h
    kconfigplugin.cpp
    kconfigplugin.h
    )

add_library(kconfigplugin ${kconfigplugin_SRCS})
target_link_libraries(kconfigplugin
        Qt5::Core
        Qt5::Qml
        KF5::ConfigCore
)

install(TARGETS kconfigplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

