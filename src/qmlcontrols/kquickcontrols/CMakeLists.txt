project(kquickcontrols)

ecm_add_qml_module(kquickcontrolsplugin URI "org.kde.kquickcontrols" VERSION 2.0)

ecm_target_qml_sources(kquickcontrolsplugin SOURCES
    KeySequenceItem.qml
    ColorButton.qml
)

ecm_finalize_qml_module(kquickcontrolsplugin DESTINATION ${KDE_INSTALL_QMLDIR})

add_subdirectory(private)

