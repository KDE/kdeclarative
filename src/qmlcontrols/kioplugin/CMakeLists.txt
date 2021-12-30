project(kioplugin)

set(kioplugin_SRCS
    kioplugin.cpp
    kioplugin.h
    krunproxy.cpp
    krunproxy.h
    )

add_library(kio ${kioplugin_SRCS})
target_link_libraries(kio
        Qt5::Core
        Qt5::Qml
        KF5::KIOWidgets
        KF5::Notifications
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kio PROPERTIES PREFIX "")
endif()

install(TARGETS kio DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kio)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kio)

