project(kcoreaddons)

set(kcoreaddonsplugin_SRCS
    kcoreaddonsplugin.cpp
    formats.cpp
    kuserproxy.cpp
    )

add_library(kcoreaddonsplugin SHARED ${kcoreaddonsplugin_SRCS})
target_link_libraries(kcoreaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        KF5::CoreAddons
)

install(TARGETS kcoreaddonsplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/kcoreaddons)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kcoreaddons)

