project(kquickcontrolsprivate)

ecm_add_qml_module(kquickcontrolsprivateplugin URI "org.kde.private.kquickcontrols" VERSION 2.0)

target_sources(kquickcontrolsprivateplugin PRIVATE
    keysequencehelper.cpp
    keysequencehelper.h
    kquickcontrolsprivateplugin.cpp
    kquickcontrolsprivateplugin.h
    translationcontext.cpp
    translationcontext.h

)

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

ecm_finalize_qml_module(kquickcontrolsprivateplugin DESTINATION ${KDE_INSTALL_QMLDIR})
