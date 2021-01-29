

set(kpackagelauncherqml_SRCS
    main.cpp
)

add_executable(kpackagelauncherqml ${kpackagelauncherqml_SRCS})

#find_package(ActiveApp REQUIRED)

target_link_libraries(kpackagelauncherqml
 Qt5::Gui
 Qt5::Quick
 Qt5::Widgets #needs a qapplication for the Desktop QtControlsStyle
 KF5::Declarative
 KF5::I18n
 KF5::Package
 KF5::QuickAddons
)

install(TARGETS kpackagelauncherqml ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
