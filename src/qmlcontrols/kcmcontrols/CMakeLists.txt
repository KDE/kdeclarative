project(kcmcontrols)

set(kcmcontrols_SRCS
    kcmcontrolsplugin.cpp
    kcmcontrolsplugin.h
    settinghighlighterprivate.cpp
    settinghighlighterprivate.h
    settingshighlighterprivate.h
    settingstateproxy.cpp
    settingstateproxy.h
)

add_library(kcmcontrolsplugin ${kcmcontrols_SRCS})
target_link_libraries(kcmcontrolsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::CoreAddons
        KF5::QuickAddons
        KF5::ConfigCore
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kcmcontrolsplugin PROPERTIES PREFIX "")
endif()


install(TARGETS kcmcontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)
