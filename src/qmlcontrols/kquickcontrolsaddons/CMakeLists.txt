project(kquickcontrolsaddons)

configure_file(config-kquickcontrolsaddons.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kquickcontrolsaddons.h)

set(kquickcontrolsaddons_SRCS
    kquickcontrolsaddonsplugin.cpp
    qpixmapitem.cpp
    qimageitem.cpp
    qiconitem.cpp
    fallbacktaphandler.cpp
    mouseeventlistener.cpp
    columnproxymodel.cpp
    clipboard.cpp
    eventgenerator.cpp
    mimedatabase.cpp
    kcmshell.cpp
    icondialog.cpp
)

add_library(kquickcontrolsaddonsplugin SHARED ${kquickcontrolsaddons_SRCS})

target_link_libraries(kquickcontrolsaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::IconThemes
        KF5::KIOWidgets
        KF5::QuickAddons
        KF5::ConfigCore)

if (HAVE_EPOXY)
    target_sources(kquickcontrolsaddonsplugin PRIVATE plotter.cpp)
    target_link_libraries(kquickcontrolsaddonsplugin epoxy::epoxy)
endif()

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
