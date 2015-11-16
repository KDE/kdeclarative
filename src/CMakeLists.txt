if(KF5Config_FOUND AND KF5I18n_FOUND AND KF5IconThemes_FOUND AND KF5KIO_FOUND)
    add_subdirectory(kdeclarative)
endif()

add_subdirectory(quickaddons)
add_subdirectory(qmlcontrols)
add_subdirectory(kpackagelauncherqml)
add_subdirectory(calendarevents)
