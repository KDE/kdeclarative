

set(qmlpackagelauncher_SRCS
    main.cpp
)

add_executable(qmlpackagelauncher ${qmlpackagelauncher_SRCS})

#find_package(ActiveApp REQUIRED)

target_link_libraries(qmlpackagelauncher
 Qt5::Gui
 Qt5::Quick
 KF5::Declarative
 KF5::I18n
 KF5::Package
 KF5::QuickAddons
)

install(TARGETS qmlpackagelauncher ${INSTALL_TARGETS_DEFAULT_ARGS})
