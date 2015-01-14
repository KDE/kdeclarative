project(kquickcontrols)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrols)
install(FILES KeySequenceItem.qml TextField.qml TextArea.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrols)

add_subdirectory(private)

