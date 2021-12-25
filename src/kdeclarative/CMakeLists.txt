
set(kdeclarative_SRCS
    configpropertymap.cpp
    configpropertymap.h
    kdeclarative.cpp
    kdeclarative.h
    private/kdeclarative_p.h
    private/kiconprovider.cpp
    private/kiconprovider_p.h
    qmlobject.cpp
    qmlobject.h
    qmlobjectsharedengine.cpp
    qmlobjectsharedengine.h

)
if(NOT ANDROID)
    list(APPEND kdeclarative_SRCS private/kioaccessmanagerfactory.cpp private/kioaccessmanagerfactory_p.h)
endif()

add_library(KF5Declarative ${kdeclarative_SRCS})
add_library(KF5::Declarative ALIAS KF5Declarative)
ecm_generate_export_header(KF5Declarative
    EXPORT_FILE_NAME ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h
    BASE_NAME KDeclarative
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.0 5.45 5.75
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

# Apps must include <KDeclarative/File> or <kdeclarative/file.h>
target_include_directories(KF5Declarative INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative>")

target_link_libraries(KF5Declarative
  PUBLIC
    KF5::ConfigCore # KCoreConfigSkeleton, in ConfigPropertyMap
    Qt5::Qml
    KF5::Package
  PRIVATE
    Qt5::Quick  #QQuickImageProvider
    KF5::I18n       #i18n bindings
    KF5::KIOWidgets     #KIO::Integration::AccessManager
    KF5::IconThemes #KIconProvider
)
set(KDeclarative_BUILD_INCLUDE_DIRS ${KDeclarative_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(KF5Declarative PUBLIC "$<BUILD_INTERFACE:${KDeclarative_BUILD_INCLUDE_DIRS}>")

set_target_properties(KF5Declarative
  PROPERTIES VERSION ${KDECLARATIVE_VERSION}
  SOVERSION ${KDECLARATIVE_SOVERSION}
  EXPORT_NAME Declarative
)

# Install files
ecm_generate_headers(KDeclarative_CamelCase_HEADERS
  HEADER_NAMES
  KDeclarative
  QmlObject
  QmlObjectSharedEngine
  ConfigPropertyMap

  PREFIX KDeclarative
  REQUIRED_HEADERS KDeclarative_HEADERS
)
install(FILES ${KDeclarative_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/KDeclarative COMPONENT Devel)

install(TARGETS KF5Declarative
  EXPORT KF5DeclarativeTargets
  ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
  ${KDeclarative_BINARY_DIR}/kdeclarative/kdeclarative_export.h
  ${KDeclarative_HEADERS}
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative/kdeclarative
  COMPONENT Devel
)

# make available to ecm_add_qch in parent folder
set(KDeclarative_QCH_SOURCES ${KDeclarative_HEADERS} PARENT_SCOPE)
set(KDeclarative_QCH_INCLUDE_DIRS ${KDeclarative_BUILD_INCLUDE_DIRS} PARENT_SCOPE)

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME KDeclarative LIB_NAME KF5Declarative DEPS "qml" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KDeclarative)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
