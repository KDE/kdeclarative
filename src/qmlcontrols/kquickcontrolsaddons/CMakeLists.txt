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
    qimageitem.cpp
    qimageitem.h
    qpixmapitem.cpp
    qpixmapitem.h

)

add_library(kquickcontrolsaddonsplugin ${kquickcontrolsaddons_SRCS})

target_link_libraries(kquickcontrolsaddonsplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Gui
        KF6::IconThemes
        KF6::KIOGui
        KF6::Service
        KF6::QuickAddons
        KF6::ConfigCore)

if (HAVE_EPOXY)
    target_sources(kquickcontrolsaddonsplugin PRIVATE plotter.cpp plotter.h)
    target_link_libraries(kquickcontrolsaddonsplugin epoxy::epoxy)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kquickcontrolsaddonsplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
