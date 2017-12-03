project(draganddrop)

set(declarativedragdrop_SRCS
    kcmcontrolsplugin.cpp
)

add_library(kcmcontrolsplugin SHARED ${declarativedragdrop_SRCS})
target_link_libraries(kcmcontrolsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::CoreAddons
        KF5::QuickAddons
)

install(TARGETS kcmcontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)
