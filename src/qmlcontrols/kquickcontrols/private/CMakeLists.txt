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
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::I18n
        KF5::ConfigGui
        KF5::GuiAddons
        KF5::WidgetsAddons
        KF5::WindowSystem
        )

if (NOT WIN32)
    target_link_libraries(kquickcontrolsprivateplugin KF5::GlobalAccel)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kquickcontrolsprivateplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kquickcontrolsprivateplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/private/kquickcontrols)
