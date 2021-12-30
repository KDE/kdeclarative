project(kcoreaddons)

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
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        KF5::CoreAddons
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kcoreaddonsplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kcoreaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kcoreaddons)

