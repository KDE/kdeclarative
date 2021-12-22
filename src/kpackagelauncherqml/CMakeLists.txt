

set(kpackagelauncherqml_SRCS
    main.cpp
)

add_executable(kpackagelauncherqml ${kpackagelauncherqml_SRCS})

#find_package(ActiveApp REQUIRED)

target_link_libraries(kpackagelauncherqml
 Qt${QT_MAJOR_VERSION}::Gui
 Qt${QT_MAJOR_VERSION}::Quick
 Qt${QT_MAJOR_VERSION}::Widgets #needs a qapplication for the Desktop QtControlsStyle
 KF5::Declarative
 KF5::I18n
 KF5::Package
 KF5::QuickAddons
)

install(TARGETS kpackagelauncherqml ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
