project(kcoreaddons)

ecm_add_qml_module(kcoreaddonsplugin URI "org.kde.kcoreaddons" VERSION 1.0)

target_sources(kcoreaddonsplugin PRIVATE
    formats.cpp
    formats.h
    kcoreaddonsplugin.cpp
    kcoreaddonsplugin.h
    kuserproxy.cpp
    kuserproxy.h
    )

target_link_libraries(kcoreaddonsplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::CoreAddons
)

ecm_finalize_qml_module(kcoreaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR})
