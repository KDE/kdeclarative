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
    kcmshell.cpp
    icondialog.cpp
)

if (NOT WIN32)
    set(kquickcontrolsaddons_SRCS ${kquickcontrolsaddons_SRCS} plotter.cpp)
    set(KQUICKCONTROLSADDONS_EXTRA_LIBS ${epoxy_LIBRARY})
    include_directories(${epoxy_INCLUDE_DIR})
endif()

add_library(kquickcontrolsaddonsplugin MODULE ${kquickcontrolsaddons_SRCS})

target_link_libraries(kquickcontrolsaddonsplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
        KF5::IconThemes
        KF5::QuickAddons
        ${KQUICKCONTROLSADDONS_EXTRA_LIBS})

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
