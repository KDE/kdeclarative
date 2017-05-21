if(KF5Config_FOUND AND KF5I18n_FOUND AND KF5IconThemes_FOUND AND KF5KIO_FOUND)
    add_subdirectory(kdeclarative)
endif()

add_subdirectory(quickaddons)
add_subdirectory(qmlcontrols)
add_subdirectory(kpackagelauncherqml)
add_subdirectory(calendarevents)

if(BUILD_QCH)
    ecm_add_qch(
        KF5Declarative_QCH
        NAME KDeclarative
        BASE_NAME KF5Declarative
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KQuickAddons_QCH_SOURCES}
            ${KDeclarative_QCH_SOURCES}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Qml_QCH
            KF5ConfigCore_QCH
            KF5Package_QCH
        BLANK_MACROS
            KDECLARATIVE_EXPORT
            KDECLARATIVE_DEPRECATED
            KDECLARATIVE_DEPRECATED_EXPORT
            QUICKADDONS_EXPORT
            QUICKADDONS_DEPRECATED
            QUICKADDONS_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
