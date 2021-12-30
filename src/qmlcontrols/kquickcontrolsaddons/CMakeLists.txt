project(kquickcontrolsaddons)

configure_file(config-kquickcontrolsaddons.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kquickcontrolsaddons.h)

set(kquickcontrolsaddons_SRCS
    clipboard.cpp
    clipboard.h
    columnproxymodel.cpp
    columnproxymodel.h
    eventgenerator.cpp
    eventgenerator.h
    fallbacktaphandler.cpp
    fallbacktaphandler.h
    icondialog.cpp
    icondialog.h
    kcmshell.cpp
    kcmshell.h
    kquickcontrolsaddonsplugin.cpp
    kquickcontrolsaddonsplugin.h
    mimedatabase.cpp
    mimedatabase.h
    mouseeventlistener.cpp
    mouseeventlistener.h
    qiconitem.cpp
    qiconitem.h
    qimageitem.cpp
    qimageitem.h
    qpixmapitem.cpp
    qpixmapitem.h

)

add_library(kquickcontrolsaddonsplugin ${kquickcontrolsaddons_SRCS})

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
    target_sources(kquickcontrolsaddonsplugin PRIVATE plotter.cpp plotter.h)
    target_link_libraries(kquickcontrolsaddonsplugin epoxy::epoxy)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kquickcontrolsaddonsplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
