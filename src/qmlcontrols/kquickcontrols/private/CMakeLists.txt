project(kquickcontrolsprivate)

set(kquickcontrolsprivate_SRCS
    keysequencehelper.cpp
    keysequencehelper.h
    kquickcontrolsprivateplugin.cpp
    kquickcontrolsprivateplugin.h
    translationcontext.cpp
    translationcontext.h

)

add_library(kquickcontrolsprivateplugin ${kquickcontrolsprivate_SRCS})

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

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kquickcontrolsprivateplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kquickcontrolsprivateplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)
