set(kwindowsystemplugin_SRCS
    kwindowsystemplugin.cpp
    kwindowsystemplugin.h
    kwindowsystemproxy.cpp
    kwindowsystemproxy.h

    )

add_library(kwindowsystem ${kwindowsystemplugin_SRCS})
target_link_libraries(kwindowsystem
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Qml
        KF5::WindowSystem
)

target_include_directories(kwindowsystem PRIVATE ${CMAKE_BINARY_DIR})

if(WIN32 AND BUILD_SHARED_LIBS)
    set_target_properties(kwindowsystem PROPERTIES PREFIX "")
endif()

install(TARGETS kwindowsystem DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)

install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kwindowsystem)
