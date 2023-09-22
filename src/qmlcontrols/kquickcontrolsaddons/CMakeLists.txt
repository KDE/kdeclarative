ecm_add_qml_module(kquickcontrolsaddonsplugin URI org.kde.kquickcontrolsaddons VERSION 2.0)

target_sources(kquickcontrolsaddonsplugin PRIVATE
    clipboard.cpp
    clipboard.h
    columnproxymodel.cpp
    columnproxymodel.h
    eventgenerator.cpp
    eventgenerator.h
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

target_link_libraries(kquickcontrolsaddonsplugin PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    KF6::ConfigCore
)

ecm_finalize_qml_module(kquickcontrolsaddonsplugin)
