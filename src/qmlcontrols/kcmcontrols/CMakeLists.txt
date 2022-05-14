project(kcmcontrols)

ecm_add_qml_module(kcmcontrolsplugin URI "org.kde.kcm" VERSION 1.1)

target_sources(kcmcontrolsplugin PRIVATE
    kcmcontrolsplugin.cpp
    kcmcontrolsplugin.h
    settinghighlighterprivate.cpp
    settinghighlighterprivate.h
    settingstateproxy.cpp
    settingstateproxy.h
)

ecm_target_qml_sources(kcmcontrolsplugin SOURCES
    qml/GridDelegate.qml
    qml/GridViewKCM.qml
    qml/GridView.qml
    qml/SimpleKCM.qml
)

ecm_target_qml_sources(kcmcontrolsplugin VERSION 1.2 SOURCES
    qml/ScrollView.qml
    qml/ScrollViewKCM.qml
)

ecm_target_qml_sources(kcmcontrolsplugin VERSION 1.3 SOURCES
    qml/SettingStateBinding.qml
)

ecm_target_qml_sources(kcmcontrolsplugin VERSION 1.4 SOURCES
    qml/AbstractKCM.qml
)

ecm_target_qml_sources(kcmcontrolsplugin VERSION 1.5 SOURCES
    qml/SettingHighlighter.qml
)

ecm_target_qml_sources(kcmcontrolsplugin VERSION 1.6 SOURCES
    qml/ContextualHelpButton.qml
)

ecm_target_qml_sources(kcmcontrolsplugin PRIVATE PATH private SOURCES
    qml/private/GridViewInternal.qml
)

target_link_libraries(kcmcontrolsplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Gui
        KF5::CoreAddons
        KF5::QuickAddons
        KF5::ConfigCore
)

ecm_finalize_qml_module(kcmcontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
