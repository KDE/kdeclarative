set(kconfigplugin_SRCS
    kauthorizedproxy.cpp
    kauthorizedproxy.h
    kconfigplugin.cpp
    kconfigplugin.h
    )

add_library(kconfigplugin ${kconfigplugin_SRCS})
target_link_libraries(kconfigplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::ConfigCore
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kconfigplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kconfigplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kconfig)

