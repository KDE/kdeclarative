project(kcoreaddons)

set(kcoreaddonsplugin_SRCS
    kcoreaddonsplugin.cpp
    formats.cpp
    kuserproxy.cpp
    )

add_library(kcoreaddonsplugin SHARED ${kcoreaddonsplugin_SRCS})
target_link_libraries(kcoreaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        KF5::CoreAddons
)

install(TARGETS kcoreaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

