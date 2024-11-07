install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrols)
install(FILES KeySequenceItem.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrols)
install(FILES ColorButton.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrols)

add_subdirectory(private)

add_library(kquickcontrolsdocumentationdummy OBJECT)

ecm_generate_qdoc(kquickcontrolsdocumentationdummy kquickcontrols.qdocconf)
