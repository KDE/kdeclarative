project(kwindowsystemplugin)

ecm_add_qml_module(kwindowsystem URI "org.kde.kwindowsystem" VERSION 1.0)

target_sources(kwindowsystem PRIVATE
    kwindowsystemplugin.cpp
    kwindowsystemplugin.h
    kwindowsystemproxy.cpp
    kwindowsystemproxy.h

    )

target_link_libraries(kwindowsystem
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::WindowSystem
)

ecm_finalize_qml_module(kwindowsystem DESTINATION ${KDE_INSTALL_QMLDIR})
