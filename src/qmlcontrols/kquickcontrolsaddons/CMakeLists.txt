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
        Qt5::Gui)

install(TARGETS kquickcontrolsaddonsplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/kquickcontrolsaddons)

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/kquickcontrolsaddons)
