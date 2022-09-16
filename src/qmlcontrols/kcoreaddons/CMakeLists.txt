set(kcoreaddonsplugin_SRCS
    formats.cpp
    formats.h
    kcoreaddonsplugin.cpp
    kcoreaddonsplugin.h
    kuserproxy.cpp
    kuserproxy.h

    )

add_library(kcoreaddonsplugin ${kcoreaddonsplugin_SRCS})
target_link_libraries(kcoreaddonsplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::CoreAddons
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kcoreaddonsplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kcoreaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

