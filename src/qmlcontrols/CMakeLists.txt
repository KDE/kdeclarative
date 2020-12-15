
add_subdirectory(draganddrop)
add_subdirectory(kquickcontrolsaddons)
add_subdirectory(kcmcontrols)
add_subdirectory(graphicaleffects)

if(KF5I18n_FOUND AND KF5Config_FOUND AND KF5WidgetsAddons_FOUND AND KF5WindowSystem_FOUND AND KF5GlobalAccel_FOUND AND KF5KIO_FOUND AND KF5GuiAddons_FOUND)
    add_subdirectory(kquickcontrols)
endif()

if(KF5CoreAddons_FOUND)
    add_subdirectory(kcoreaddons)
endif()


if(KF5KIO_FOUND)
    add_subdirectory(kioplugin)
endif()

if (KF5WindowSystem_FOUND)
    add_subdirectory(kwindowsystemplugin)
endif()

if (KF5Config_FOUND)
    add_subdirectory(kconfig)
endif()
