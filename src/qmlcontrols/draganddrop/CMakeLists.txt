ecm_add_qml_module(draganddropplugin URI org.kde.draganddrop VERSION 2.0 DEPENDENCIES QtQuick GENERATE_PLUGIN_SOURCE)

ecm_generate_qdoc(draganddropplugin draganddrop.qdocconf)

target_sources(draganddropplugin PRIVATE
    DeclarativeDragArea.cpp
    DeclarativeDragArea.h
    DeclarativeDragDropEvent.cpp
    DeclarativeDragDropEvent.h
    DeclarativeDropArea.cpp
    DeclarativeDropArea.h
    DeclarativeMimeData.cpp
    DeclarativeMimeData.h
    MimeDataWrapper.cpp
    MimeDataWrapper.h
)

target_link_libraries(draganddropplugin PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
)

ecm_finalize_qml_module(draganddropplugin)
