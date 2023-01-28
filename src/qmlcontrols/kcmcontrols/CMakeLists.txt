set(kcmcontrols_SRCS
    kcmcontrolsplugin.cpp
    kcmcontrolsplugin.h
    settinghighlighterprivate.cpp
    settinghighlighterprivate.h
    settingstateproxy.cpp
    settingstateproxy.h
)

add_library(kcmcontrolsplugin ${kcmcontrols_SRCS})
target_link_libraries(kcmcontrolsplugin
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Gui
        KF6::CoreAddons
        KF6::QuickAddons
        KF6::ConfigCore
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kcmcontrolsplugin PROPERTIES PREFIX "")
endif()


install(TARGETS kcmcontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)

install(DIRECTORY qml/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcm)
