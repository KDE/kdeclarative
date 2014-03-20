
add_subdirectory(draganddrop)
add_subdirectory(qtextracomponents)

if(KF5I18n_FOUND AND KF5ConfigGui_FOUND AND KF5WidgetsAddons_FOUND AND KF5WindowSystem_FOUND AND KF5GlobalAccel_FOUND AND KF5KIO_FOUND AND KF5GuiAddons_FOUND)
    add_subdirectory(kquickcontrols)
endif()

