set(kwindowsystemplugin_SRCS
    kwindowsystemplugin.cpp
    kwindowsystemplugin.h
    kwindowsystemproxy.cpp
    kwindowsystemproxy.h

    )

add_library(kwindowsystem ${kwindowsystemplugin_SRCS})
target_link_libraries(kwindowsystem
        Qt6::Core
        Qt6::Qml
        KF6::WindowSystem
)

target_include_directories(kwindowsystem PRIVATE ${CMAKE_BINARY_DIR})

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kwindowsystem PROPERTIES PREFIX "")
endif()

if (UNIX AND NOT ANDROID AND NOT APPLE)
    set(HAVE_X11 TRUE)
else()
    set(HAVE_X11 FALSE)
endif()

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

install(TARGETS kwindowsystem DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)
