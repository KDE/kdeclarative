find_package(Qt6 REQUIRED COMPONENTS ShaderTools)

qt_add_qml_module(graphicaleffects URI org.kde.graphicaleffects
    PLUGIN_TARGET graphicaleffects
    VERSION 1.0
    QML_FILES
        Lanczos.qml
        BadgeEffect.qml
)

set_target_properties(graphicaleffects PROPERTIES PREFIX "lib")

qt_add_shaders(graphicaleffects "graphicaleffects_shaders"
    BATCHABLE
    PRECOMPILE
    OPTIMIZED
    PREFIX
        "/shaders"
    FILES
       "badge.frag"
       "preserveaspect.vert"
       "lanczos2sharp.frag"
)

qt_query_qml_module(graphicaleffects
    URI module_uri
    VERSION module_version
    PLUGIN_TARGET module_plugin_target
    TARGET_PATH module_target_path
    QMLDIR module_qmldir
    TYPEINFO module_typeinfo
    QML_FILES module_qml_files
    RESOURCES module_resources
)

set(module_dir ${KDE_INSTALL_QMLDIR}/org/kde/graphicaleffects)

install(TARGETS "${module_plugin_target}"
    LIBRARY DESTINATION "${module_dir}"
    RUNTIME DESTINATION "${module_dir}"
)

install(FILES "${module_qmldir}" DESTINATION "${module_dir}")
install(FILES "${module_typeinfo}" DESTINATION "${module_dir}")
install(FILES ${module_qml_files} DESTINATION "${module_dir}")

