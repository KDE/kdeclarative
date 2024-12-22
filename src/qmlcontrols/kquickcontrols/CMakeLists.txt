add_subdirectory(private)

ecm_add_qml_module(kquickcontrols URI "org.kde.kquickcontrols" VERSION 2.0 GENERATE_PLUGIN_SOURCE)

target_sources(kquickcontrols PRIVATE types.h)

ecm_target_qml_sources(kquickcontrols SOURCES ColorButton.qml KeySequenceItem.qml)

ecm_finalize_qml_module(kquickcontrols)


target_link_libraries(kquickcontrols PRIVATE kquickcontrolsprivate)
