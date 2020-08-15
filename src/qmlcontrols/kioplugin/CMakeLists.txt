project(kioplugin)

set(kioplugin_SRCS
    kioplugin.cpp
    krunproxy.cpp
    )

add_library(kio SHARED ${kioplugin_SRCS})
target_link_libraries(kio
        Qt5::Core
        Qt5::Qml
        KF5::KIOWidgets
        KF5::Notifications
)

install(TARGETS kio DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kio)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kio)

