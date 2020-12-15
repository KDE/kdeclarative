project(kgraphicaleffects)

install(FILES
    qmldir
    Lanczos.qml
    lanczos2sharp.glsl
    lanczos2sharp_core.glsl
    DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/graphicaleffects
)
