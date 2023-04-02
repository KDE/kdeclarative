set(calendar-integration_SRCS
    calendareventsplugin.cpp
    calendareventsplugin.h
    eventdata_p.cpp
)

add_library(KF5CalendarEvents ${calendar-integration_SRCS})
add_library(KF5::CalendarEvents ALIAS KF5CalendarEvents)

ecm_generate_export_header(KF5CalendarEvents
    BASE_NAME CalendarEvents
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.106
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

target_link_libraries(KF5CalendarEvents PUBLIC Qt${QT_MAJOR_VERSION}::Core)

target_include_directories(KF5CalendarEvents INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative>")
set_target_properties(KF5CalendarEvents PROPERTIES VERSION ${KDECLARATIVE_VERSION}
                                                SOVERSION ${KDECLARATIVE_SOVERSION}
                                                EXPORT_NAME CalendarEvents
)

install(TARGETS KF5CalendarEvents EXPORT KF5DeclarativeTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(CalendarEvents_CamelCase_HEADERS
  HEADER_NAMES
  CalendarEventsPlugin

  PREFIX CalendarEvents
  REQUIRED_HEADERS CalendarEvents_HEADERS
)
install(FILES ${CalendarEvents_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/CalendarEvents COMPONENT Devel)

install (FILES
        ${CalendarEvents_HEADERS}
	${CMAKE_CURRENT_BINARY_DIR}/calendarevents_export.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KDeclarative/calendarevents COMPONENT Devel
)

# make available to ecm_add_qch in parent folder
set(CalendarEvents_QCH_SOURCES ${CalendarEvents_HEADERS} PARENT_SCOPE)
set(CalendarEvents_QCH_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
