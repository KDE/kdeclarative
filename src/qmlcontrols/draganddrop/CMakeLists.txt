project(draganddrop)

ecm_add_qml_module(draganddropplugin URI "org.kde.draganddrop" VERSION 1.0)

target_sources(draganddropplugin PRIVATE
    DeclarativeDragArea.cpp
    DeclarativeDragArea.h
    DeclarativeDragDropEvent.cpp
    DeclarativeDragDropEvent.h
    DeclarativeDropArea.cpp
    DeclarativeDropArea.h
    DeclarativeMimeData.cpp
    DeclarativeMimeData.h
    draganddropplugin.cpp
    draganddropplugin.h
    MimeDataWrapper.cpp
    MimeDataWrapper.h
)

target_link_libraries(draganddropplugin
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Gui
)

ecm_finalize_qml_module(draganddropplugin DESTINATION ${KDE_INSTALL_QMLDIR})
