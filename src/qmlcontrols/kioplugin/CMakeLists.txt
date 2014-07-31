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
)

install(TARGETS kio DESTINATION ${QML_INSTALL_DIR}/org/kde/kio)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kio)

