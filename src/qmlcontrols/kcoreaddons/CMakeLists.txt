project(kcoreaddons)

set(kcoreaddonsplugin_SRCS
    kcoreaddonsplugin.cpp
    formats.cpp
    )

add_library(kcoreaddonspplugin SHARED ${kcoreaddonspplugin_SRCS})
target_link_libraries(kcoreaddonspplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        KF5::CoreAddons
)

install(TARGETS kcoreaddonspplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/kcoreaddons)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kcoreaddons)

