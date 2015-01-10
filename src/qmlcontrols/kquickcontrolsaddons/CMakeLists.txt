project(kquickcontrolsaddons)

set(kquickcontrolsaddons_SRCS
    kquickcontrolsaddonsplugin.cpp
    qpixmapitem.cpp
    qimageitem.cpp
    qiconitem.cpp
    mouseeventlistener.cpp
    columnproxymodel.cpp
    clipboard.cpp
    mimedatabase.cpp
)

add_library(kquickcontrolsaddonsplugin SHARED ${kquickcontrolsaddons_SRCS})

target_link_libraries(kquickcontrolsaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::QuickAddons)

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
