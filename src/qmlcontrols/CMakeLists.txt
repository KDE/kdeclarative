
add_subdirectory(draganddrop)
add_subdirectory(kquickcontrolsaddons)
add_subdirectory(kcmcontrols)
add_subdirectory(graphicaleffects)
add_subdirectory(kquickcontrols)

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
