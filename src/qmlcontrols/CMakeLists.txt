
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


# TODO KF6 remove the KIO plugin
if(KF5KIO_FOUND)
    if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
    EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.89.0)
        add_subdirectory(kioplugin)
    endif()
endif()

if (KF5WindowSystem_FOUND)
    add_subdirectory(kwindowsystemplugin)
endif()

if (KF5Config_FOUND)
    add_subdirectory(kconfig)
endif()
