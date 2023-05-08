set(kquickcontrolsaddons_SRCS
    clipboard.cpp
    clipboard.h
    columnproxymodel.cpp
    columnproxymodel.h
    eventgenerator.cpp
    eventgenerator.h
    fallbacktaphandler.cpp
    fallbacktaphandler.h
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
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    KF6::ConfigCore
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kquickcontrolsaddonsplugin PROPERTIES PREFIX "")
endif()

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kquickcontrolsaddons)
