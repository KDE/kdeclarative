project(draganddrop)

set(declarativedragdrop_SRCS
    draganddropplugin.cpp
    DeclarativeDragArea.cpp
    DeclarativeDragDropEvent.cpp
    DeclarativeDropArea.cpp
    DeclarativeMimeData.cpp
    MimeDataWrapper.cpp
)

add_library(draganddropplugin SHARED ${declarativedragdrop_SRCS})
target_link_libraries(draganddropplugin
        Qt5::Core
        Qt5::Quick
        Qt5::Qml
        Qt5::Gui
)

install(TARGETS draganddropplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)
