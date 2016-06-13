project(kquickcontrolsaddons)

configure_file(config-kquickcontrolsaddons.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-kquickcontrolsaddons.h)

set(kquickcontrolsaddons_SRCS
    kquickcontrolsaddonsplugin.cpp
    qpixmapitem.cpp
    qimageitem.cpp
    qiconitem.cpp
    mouseeventlistener.cpp
    columnproxymodel.cpp
    clipboard.cpp
    eventgenerator.cpp
    mimedatabase.cpp
    kcmshell.cpp
    icondialog.cpp
)

if (HAVE_EPOXY)
    set(kquickcontrolsaddons_SRCS ${kquickcontrolsaddons_SRCS} plotter.cpp)
    set(KQUICKCONTROLSADDONS_EXTRA_LIBS ${epoxy_LIBRARY})
    include_directories(${epoxy_INCLUDE_DIR})
endif()

add_library(kquickcontrolsaddonsplugin SHARED ${kquickcontrolsaddons_SRCS})

target_link_libraries(kquickcontrolsaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::IconThemes
        KF5::QuickAddons
        KF5::ConfigCore
        ${KQUICKCONTROLSADDONS_EXTRA_LIBS})

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
