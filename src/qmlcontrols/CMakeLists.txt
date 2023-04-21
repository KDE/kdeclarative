
add_subdirectory(draganddrop)
add_subdirectory(kquickcontrolsaddons)
add_subdirectory(kcmcontrols)
add_subdirectory(graphicaleffects)

if(KF6I18n_FOUND AND KF6Config_FOUND AND KF6WidgetsAddons_FOUND AND KF6GlobalAccel_FOUND AND KF6KIO_FOUND AND KF6GuiAddons_FOUND)
    add_subdirectory(kquickcontrols)
endif()

if(KF6CoreAddons_FOUND)
    add_subdirectory(kcoreaddons)
endif()

