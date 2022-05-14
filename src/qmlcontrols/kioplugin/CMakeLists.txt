project(kioplugin)

ecm_add_qml_module(kio URI "org.kde.kio" VERSION 1.0)

target_sources(kio PRIVATE
    kioplugin.cpp
    kioplugin.h
    krunproxy.cpp
    krunproxy.h
)

target_link_libraries(kio
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::KIOWidgets
        KF5::Notifications
)

ecm_finalize_qml_module(kio DESTINATION ${KDE_INSTALL_QMLDIR})


