add_subdirectory(qmlcontrols)
add_subdirectory(calendarevents)

if(BUILD_QCH)
    ecm_add_qch(
        KF6Declarative_QCH
        NAME KDeclarative
        BASE_NAME KF6Declarative
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KQuickAddons_QCH_SOURCES}
            ${KDeclarative_QCH_SOURCES}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt6Qml_QCH
            KF6Config_QCH
            KF6Package_QCH
        INCLUDE_DIRS
            ${KQuickAddons_QCH_INCLUDE_DIRS}
            ${KDeclarative_QCH_INCLUDE_DIRS}
        BLANK_MACROS
            KDECLARATIVE_EXPORT
            KDECLARATIVE_DEPRECATED
            KDECLARATIVE_DEPRECATED_EXPORT
            "KDECLARATIVE_DEPRECATED_VERSION(x, y, t)"
            "KDECLARATIVE_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
            QUICKADDONS_EXPORT
            QUICKADDONS_DEPRECATED
            QUICKADDONS_DEPRECATED_EXPORT
            "QUICKADDONS_DEPRECATED_VERSION(x, y, t)"
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
