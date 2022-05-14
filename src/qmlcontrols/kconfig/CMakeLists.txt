project(kconfigplugin)

ecm_add_qml_module(kconfigplugin URI "org.kde.kconfig" VERSION 1.0)

target_sources(kconfigplugin PRIVATE
    kauthorizedproxy.cpp
    kauthorizedproxy.h
    kconfigplugin.cpp
    kconfigplugin.h
    )

target_link_libraries(kconfigplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::ConfigCore
)

ecm_finalize_qml_module(kconfigplugin DESTINATION ${KDE_INSTALL_QMLDIR})
