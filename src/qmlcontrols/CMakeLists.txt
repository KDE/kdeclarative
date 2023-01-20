
add_subdirectory(draganddrop)
add_subdirectory(kquickcontrolsaddons)
add_subdirectory(kcmcontrols)
add_subdirectory(graphicaleffects)

if(KF6I18n_FOUND AND KF6Config_FOUND AND KF6WidgetsAddons_FOUND AND KF6WindowSystem_FOUND AND KF6GlobalAccel_FOUND AND KF6KIO_FOUND AND KF6GuiAddons_FOUND)
    add_subdirectory(kquickcontrols)
endif()

if(KF6CoreAddons_FOUND)
    add_subdirectory(kcoreaddons)
endif()


# TODO KF6 remove the KIO plugin
if(KF6KIO_FOUND)
    if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
    EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.89.0)
        add_subdirectory(kioplugin)
    endif()
endif()

if (KF6WindowSystem_FOUND)
    add_subdirectory(kwindowsystemplugin)
endif()

if (KF6Config_FOUND)
    add_subdirectory(kconfig)
endif()
