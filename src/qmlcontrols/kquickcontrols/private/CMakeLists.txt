project(kquickcontrolsprivate)

set(kquickcontrolsprivate_SRCS
    kquickcontrolsprivateplugin.cpp
    keysequencehelper.cpp
    translationcontext.cpp
)

add_library(kquickcontrolsprivateplugin SHARED ${kquickcontrolsprivate_SRCS})

target_link_libraries(kquickcontrolsprivateplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        KF5::I18n
        KF5::ConfigGui
        KF5::GuiAddons
        KF5::WidgetsAddons
        KF5::WindowSystem
        KF5::GlobalAccel
        )

install(TARGETS kquickcontrolsprivateplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)
