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
        Qt6::Core
        Qt6::Quick
        Qt6::Qml
        Qt6::Gui
)

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(draganddropplugin PROPERTIES PREFIX "")
endif()

install(TARGETS draganddropplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/draganddrop)
