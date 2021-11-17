project(draganddrop)

set(declarativedragdrop_SRCS
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

add_library(draganddropplugin ${declarativedragdrop_SRCS})
target_link_libraries(draganddropplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
)

install(TARGETS draganddropplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)
