set(calendar-integration_SRCS
    calendareventsplugin.cpp
    calendareventsplugin.h
    eventdata_p.cpp
)

add_library(KF5CalendarEvents ${calendar-integration_SRCS})
generate_export_header(KF5CalendarEvents BASE_NAME CalendarEvents)
add_library(KF5::CalendarEvents ALIAS KF5CalendarEvents)

target_link_libraries(KF5CalendarEvents PUBLIC Qt5::Core)

target_include_directories(KF5CalendarEvents INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative>")
set_target_properties(KF5CalendarEvents PROPERTIES VERSION ${KDECLARATIVE_VERSION}
                                                SOVERSION ${KDECLARATIVE_SOVERSION}
                                                EXPORT_NAME CalendarEvents
)

install(TARGETS KF5CalendarEvents EXPORT KF5DeclarativeTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(CalendarEvents_CamelCase_HEADERS
  HEADER_NAMES
  CalendarEventsPlugin

  PREFIX CalendarEvents
  REQUIRED_HEADERS calendareventsplugin.h
)
install(FILES ${CalendarEvents_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/CalendarEvents COMPONENT Devel)

install (FILES
        calendareventsplugin.h
	${CMAKE_CURRENT_BINARY_DIR}/calendarevents_export.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/calendarevents COMPONENT Devel
)
